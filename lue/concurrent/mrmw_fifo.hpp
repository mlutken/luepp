#pragma once

#include <atomic>
#include <memory>
#include <cstdint>
#include <optional>
#include <limits>

// ----------------------------------------------
// --- mrmw_fifo.hpp ---
// ----------------------------------------------
namespace lue {

/**
Multiple reader, multiple writer lockless fifo.
Uses atomics for the read and write indices and per-slot sequence numbers
to coordinate concurrent access.

This implementation is based on Dmitry Vyukov's bounded MPMC queue algorithm.

Note: For optimal performance in multi-threaded scenarios, use try_push() and
try_pop() methods which provide atomic push-and-publish and claim-and-pop semantics.

The push()/pop()/front() interface is provided for compatibility with srsw_fifo
but requires external synchronization when used with multiple readers or writers.

@tparam T The element type stored in the queue
@tparam Allocator The allocator type (defaults to std::allocator<T>)
@tparam ALIGN_SIZE Alignment for atomic indices to avoid false sharing (default: 128)
*/
template <typename T, class Allocator = std::allocator<T>, size_t ALIGN_SIZE = 128>
class mrmw_fifo
{
private:
    // -------------------------
    // PRIVATE: Helper structs ---
    // -------------------------
    struct alignas(alignof(T)) cell_t
    {
        std::atomic<std::size_t> sequence;
        T data;

        cell_t() : sequence(0) {}
    };

    // -------------------------
    // PRIVATE: Member data ---
    // -------------------------
    std::unique_ptr<cell_t[]> m_buffer;
    std::size_t m_capacity;
    std::size_t m_capacity_mask;

    alignas(ALIGN_SIZE) std::atomic<std::size_t> m_write_index;
    alignas(ALIGN_SIZE) std::atomic<std::size_t> m_read_index;

    // For compatibility with srsw_fifo interface
    alignas(ALIGN_SIZE) std::atomic<std::size_t> m_claimed_read_index;

public:
    // ------------------------
    // PUBLIC: Typedefs ---
    // ------------------------
    typedef T                                       value_type;
    typedef std::size_t                             size_type;
    typedef std::atomic<size_type>                  atomic_size_type;
    typedef Allocator                               allocator_type;
    typedef typename std::allocator_traits<Allocator>::difference_type   difference_type;
    typedef typename std::allocator_traits<Allocator>::pointer           pointer;
    typedef typename std::allocator_traits<Allocator>::const_pointer     const_pointer;

    // ------------------------
    // PUBLIC: Constructors ---
    // ------------------------
    mrmw_fifo()
        : m_capacity(0)
        , m_capacity_mask(0)
        , m_write_index(0)
        , m_read_index(0)
        , m_claimed_read_index(0)
    {
    }

    explicit mrmw_fifo(size_type capacity)
        : m_capacity(0)
        , m_capacity_mask(0)
        , m_write_index(0)
        , m_read_index(0)
        , m_claimed_read_index(0)
    {
        init_queue(capacity);
    }

    mrmw_fifo(size_type capacity, const allocator_type& /*allocator*/)
        : m_capacity(0)
        , m_capacity_mask(0)
        , m_write_index(0)
        , m_read_index(0)
        , m_claimed_read_index(0)
    {
        // Note: Allocator parameter is ignored for this implementation
        // as we use unique_ptr for internal storage
        init_queue(capacity);
    }

    // ------------------------
    // PUBLIC: Initialization ---
    // ------------------------
    void init_queue(size_type capacity)
    {
        // Round up to power of 2 for efficient modulo
        m_capacity = round_to_power_of_two(capacity);
        m_capacity_mask = m_capacity - 1;

        // Allocate buffer using unique_ptr (constructs cells in-place)
        m_buffer = std::make_unique<cell_t[]>(m_capacity);

        for (size_type i = 0; i < m_capacity; ++i)
        {
            m_buffer[i].sequence.store(i, std::memory_order_relaxed);
        }

        m_write_index.store(0, std::memory_order_relaxed);
        m_read_index.store(0, std::memory_order_relaxed);
        m_claimed_read_index.store(0, std::memory_order_relaxed);
    }

    void resize_queue(size_type new_capacity)
    {
        // Note: This operation is not thread-safe and should only be called
        // when no other threads are accessing the queue
        mrmw_fifo temp(new_capacity);

        while (!empty())
        {
            value_type elem;
            if (try_pop(elem))
            {
                temp.try_push(std::move(elem));
            }
        }

        swap(temp);
    }

    void swap(mrmw_fifo& other) noexcept
    {
        std::swap(m_buffer, other.m_buffer);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_capacity_mask, other.m_capacity_mask);

        auto wi = m_write_index.load(std::memory_order_relaxed);
        auto ri = m_read_index.load(std::memory_order_relaxed);
        auto cri = m_claimed_read_index.load(std::memory_order_relaxed);

        m_write_index.store(other.m_write_index.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_read_index.store(other.m_read_index.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_claimed_read_index.store(other.m_claimed_read_index.load(std::memory_order_relaxed), std::memory_order_relaxed);

        other.m_write_index.store(wi, std::memory_order_relaxed);
        other.m_read_index.store(ri, std::memory_order_relaxed);
        other.m_claimed_read_index.store(cri, std::memory_order_relaxed);
    }

    // ------------------------
    // PUBLIC: Thread-safe push operations ---
    // ------------------------

    /**
     Atomically push a value to the queue.
     @param value The value to push
     @return true if successful, false if the queue is full
     This method is safe to call from multiple writer threads concurrently.
    */
    bool try_push(const T& value)
    {
        size_type pos = m_write_index.load(std::memory_order_relaxed);

        for (;;)
        {
            cell_t& cell = m_buffer[pos & m_capacity_mask];
            size_type seq = cell.sequence.load(std::memory_order_acquire);
            difference_type diff = static_cast<difference_type>(seq) - static_cast<difference_type>(pos);

            if (diff == 0)
            {
                // Cell is ready for writing
                if (m_write_index.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                {
                    // Successfully claimed the slot
                    cell.data = value;
                    cell.sequence.store(pos + 1, std::memory_order_release);
                    return true;
                }
                // CAS failed, retry with updated pos
            }
            else if (diff < 0)
            {
                // Queue is full
                return false;
            }
            else
            {
                // Another writer claimed this slot, try next
                pos = m_write_index.load(std::memory_order_relaxed);
            }
        }
    }

    bool try_push(T&& value)
    {
        size_type pos = m_write_index.load(std::memory_order_relaxed);

        for (;;)
        {
            cell_t& cell = m_buffer[pos & m_capacity_mask];
            size_type seq = cell.sequence.load(std::memory_order_acquire);
            difference_type diff = static_cast<difference_type>(seq) - static_cast<difference_type>(pos);

            if (diff == 0)
            {
                if (m_write_index.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                {
                    cell.data = std::move(value);
                    cell.sequence.store(pos + 1, std::memory_order_release);
                    return true;
                }
            }
            else if (diff < 0)
            {
                return false;
            }
            else
            {
                pos = m_write_index.load(std::memory_order_relaxed);
            }
        }
    }

    template<class... Args>
    bool try_emplace(Args&&... args)
    {
        size_type pos = m_write_index.load(std::memory_order_relaxed);

        for (;;)
        {
            cell_t& cell = m_buffer[pos & m_capacity_mask];
            size_type seq = cell.sequence.load(std::memory_order_acquire);
            difference_type diff = static_cast<difference_type>(seq) - static_cast<difference_type>(pos);

            if (diff == 0)
            {
                if (m_write_index.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                {
                    new (&cell.data) T(std::forward<Args>(args)...);
                    cell.sequence.store(pos + 1, std::memory_order_release);
                    return true;
                }
            }
            else if (diff < 0)
            {
                return false;
            }
            else
            {
                pos = m_write_index.load(std::memory_order_relaxed);
            }
        }
    }

    // ------------------------
    // PUBLIC: Thread-safe pop operations ---
    // ------------------------

    /**
     Atomically pop a value from the queue.
     @param value Output parameter for the popped value
     @return true if successful, false if the queue is empty
     This method is safe to call from multiple reader threads concurrently.
    */
    bool try_pop(T& value)
    {
        size_type pos = m_read_index.load(std::memory_order_relaxed);

        for (;;)
        {
            cell_t& cell = m_buffer[pos & m_capacity_mask];
            size_type seq = cell.sequence.load(std::memory_order_acquire);
            difference_type diff = static_cast<difference_type>(seq) - static_cast<difference_type>(pos + 1);

            if (diff == 0)
            {
                // Cell is ready for reading
                if (m_read_index.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                {
                    // Successfully claimed the slot
                    value = std::move(cell.data);
                    cell.sequence.store(pos + m_capacity, std::memory_order_release);
                    return true;
                }
                // CAS failed, retry with updated pos
            }
            else if (diff < 0)
            {
                // Queue is empty
                return false;
            }
            else
            {
                // Another reader claimed this slot, try next
                pos = m_read_index.load(std::memory_order_relaxed);
            }
        }
    }

    /**
     Atomically pop a value from the queue and return it.
     @return std::optional containing the value if successful, empty optional if queue is empty
    */
    std::optional<T> try_pop()
    {
        T value;
        if (try_pop(value))
        {
            return std::move(value);
        }
        return std::nullopt;
    }

    // ------------------------
    // PUBLIC: Compatibility interface (similar to srsw_fifo) ---
    // ------------------------

    /**
     Push a value to the queue.
     @return true if successful, false if the queue is full
     Note: For multiple writers, prefer try_push() for better performance.
    */
    bool push(const T& value)
    {
        return try_push(value);
    }

    bool push(T&& value)
    {
        return try_push(std::move(value));
    }

    template<class... Args>
    bool emplace(Args&&... args)
    {
        return try_emplace(std::forward<Args>(args)...);
    }

    /**
     Access the front element.
     WARNING: This method is NOT thread-safe for multiple readers.
     In MRMW scenarios, use try_pop() instead.
     This is provided for compatibility with srsw_fifo interface.
    */
    T& front()
    {
        size_type pos = m_claimed_read_index.load(std::memory_order_relaxed);
        return m_buffer[pos & m_capacity_mask].data;
    }

    const T& front() const
    {
        size_type pos = m_claimed_read_index.load(std::memory_order_relaxed);
        return m_buffer[pos & m_capacity_mask].data;
    }

    /**
     Remove the front element.
     WARNING: This method is NOT thread-safe for multiple readers.
     In MRMW scenarios, use try_pop() instead.
     This is provided for compatibility with srsw_fifo interface.
    */
    bool pop()
    {
        T value;
        bool success = try_pop(value);
        if (success)
        {
            m_claimed_read_index.store(m_read_index.load(std::memory_order_relaxed), std::memory_order_relaxed);
        }
        return success;
    }

    void flush()
    {
        T value;
        while (try_pop(value))
        {
            // Discard elements
        }
        m_claimed_read_index.store(m_read_index.load(std::memory_order_relaxed), std::memory_order_relaxed);
    }

    // ------------------------
    // PUBLIC: Status queries ---
    // ------------------------

    /**
     Check if the queue is empty.
     Note: The result may be stale by the time it's used in a concurrent context.
    */
    bool empty() const
    {
        size_type write_idx = m_write_index.load(std::memory_order_acquire);
        size_type read_idx = m_read_index.load(std::memory_order_acquire);
        return write_idx == read_idx;
    }

    /**
     Check if the queue is full.
     Note: The result may be stale by the time it's used in a concurrent context.
    */
    bool full() const
    {
        size_type write_idx = m_write_index.load(std::memory_order_acquire);
        size_type read_idx = m_read_index.load(std::memory_order_acquire);
        return (write_idx - read_idx) >= m_capacity;
    }

    /**
     Get approximate size of the queue.
     Note: The result may be stale by the time it's used in a concurrent context.
    */
    size_type size() const
    {
        size_type write_idx = m_write_index.load(std::memory_order_acquire);
        size_type read_idx = m_read_index.load(std::memory_order_acquire);
        difference_type diff = static_cast<difference_type>(write_idx) - static_cast<difference_type>(read_idx);
        return diff >= 0 ? static_cast<size_type>(diff) : 0;
    }

    size_type capacity() const
    {
        return m_capacity;
    }

    size_type buffer_size() const
    {
        return m_capacity;
    }

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------

    static constexpr size_type round_to_power_of_two(size_type v)
    {
        // Minimum capacity is 2 for practical MPMC queue operation
        if (v < 2) return 2;
        if ((v & (v - 1)) == 0) return v;

        v--;
        for (size_type i = 1; i < sizeof(size_type) * 8; i <<= 1)
        {
            v |= v >> i;
        }
        v++;
        return v;
    }
};

} // END namespace lue