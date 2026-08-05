#pragma once

#include <luepp_default_config.h>
#include <atomic>
#include <cstdint>
#include <utility>

// ----------------------------------------------
// --- mrmw_fifo.hpp ---
// ----------------------------------------------
namespace lue {

/**
Multiple reader, multiple writer lockless fifo (ring buffer).
Uses atomics for the read and write indices internally and one atomic per
slot to track whether the slot is ready for reading.

Writers claim a slot by atomically advancing the write index via a
compare_exchange loop.  After writing the value they set the per-slot
ready flag (release).  Readers claim a slot by atomically advancing the
read index via a compare_exchange loop, then check the ready flag
(acquire) before consuming the value and clearing the flag (release).

@note One slot is always reserved to distinguish full from empty, so the
      usable capacity is BUFFER_SIZE - 1.
@sa lue::srsw_fifo_s for the single-reader / single-writer variant.
*/



template <typename T, size_t BUFFER_SIZE, size_t ALIGN_SIZE = 128>
class mrmw_fifo
{
    static_assert(BUFFER_SIZE > 1, "BUFFER_SIZE must be > 1");

public:
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    typedef T                       value_type;
    typedef std::size_t             size_type;
    typedef std::atomic<size_type>  atomic_size_type;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;

    mrmw_fifo()
        : m_write_index(0)
        , m_read_index(0)
    {
        for (size_type i = 0; i < BUFFER_SIZE; ++i) {
            m_ready[i].store(false, std::memory_order_relaxed);
        }
    }

    mrmw_fifo(const mrmw_fifo&) = delete;
    mrmw_fifo& operator=(const mrmw_fifo&) = delete;
    mrmw_fifo(mrmw_fifo&&) = delete;
    mrmw_fifo& operator=(mrmw_fifo&&) = delete;

    bool push(T&& v)
    {
        size_type write_index = m_write_index.load(std::memory_order_relaxed);
        while (true) {
            const size_type read_index = m_read_index.load(std::memory_order_acquire);
            const size_type next_write_index = inc_index(write_index);
            if (next_write_index == read_index) {
                return false; // full
            }
            if (m_write_index.compare_exchange_weak(
                    write_index, next_write_index,
                    std::memory_order_acq_rel,
                    std::memory_order_relaxed)) {
                // Slot claimed — write the value, then publish
                new (&m_buffer[write_index]) value_type(std::move(v));
                m_ready[write_index].store(true, std::memory_order_release);
                return true;
            }
            // CAS failed: another writer won the slot, retry with updated write_index
        }
    }

    bool push(const T& v)
    {
        size_type write_index = m_write_index.load(std::memory_order_relaxed);
        while (true) {
            const size_type read_index = m_read_index.load(std::memory_order_acquire);
            const size_type next_write_index = inc_index(write_index);
            if (next_write_index == read_index) {
                return false; // full
            }
            if (m_write_index.compare_exchange_weak(
                    write_index, next_write_index,
                    std::memory_order_acq_rel,
                    std::memory_order_relaxed)) {
                new (&m_buffer[write_index]) value_type(v);
                m_ready[write_index].store(true, std::memory_order_release);
                return true;
            }
        }
    }

    template <class... Args>
    bool emplace(Args&&... args)
    {
        size_type write_index = m_write_index.load(std::memory_order_relaxed);
        while (true) {
            const size_type read_index = m_read_index.load(std::memory_order_acquire);
            const size_type next_write_index = inc_index(write_index);
            if (next_write_index == read_index) {
                return false; // full
            }
            if (m_write_index.compare_exchange_weak(
                    write_index, next_write_index,
                    std::memory_order_acq_rel,
                    std::memory_order_relaxed)) {
                new (&m_buffer[write_index]) value_type(std::forward<Args>(args)...);
                m_ready[write_index].store(true, std::memory_order_release);
                return true;
            }
        }
    }

    bool pop()
    {
        size_type read_index = m_read_index.load(std::memory_order_relaxed);
        while (true) {
            const size_type write_index = m_write_index.load(std::memory_order_acquire);
            if (read_index == write_index) {
                return false; // empty
            }
            // Check that the writer has finished publishing this slot
            if (!m_ready[read_index].load(std::memory_order_acquire)) {
                return false; // slot not ready yet
            }
            if (m_read_index.compare_exchange_weak(
                    read_index, inc_index(read_index),
                    std::memory_order_acq_rel,
                    std::memory_order_relaxed)) {
                // Slot claimed — destroy element and clear flag
                reinterpret_cast<pointer>(&m_buffer[read_index * sizeof(T)])->~T();
                m_ready[read_index].store(false, std::memory_order_release);
                return true;
            }
        }
    }

    T& front()
    {
        const size_type read_index = m_read_index.load(std::memory_order_acquire);
        return *reinterpret_cast<pointer>(&m_buffer[read_index]);
    }

    const T& front() const
    {
        const size_type read_index = m_read_index.load(std::memory_order_acquire);
        return *reinterpret_cast<const_pointer>(&m_buffer[read_index]);
    }

    void flush()
    {
        while (pop()) {
            // keep popping
        }
    }

    bool full() const
    {
        const size_type write_index = m_write_index.load(std::memory_order_acquire);
        const size_type read_index = m_read_index.load(std::memory_order_acquire);
        return inc_index(write_index) == read_index;
    }

    bool empty() const
    {
        return m_read_index.load(std::memory_order_acquire) ==
               m_write_index.load(std::memory_order_acquire);
    }

    size_type size() const
    {
        const size_type read_index = m_read_index.load(std::memory_order_acquire);
        const size_type write_index = m_write_index.load(std::memory_order_acquire);
        std::int64_t sz = static_cast<std::int64_t>(write_index) -
                          static_cast<std::int64_t>(read_index);
        if (sz >= 0) {
            return static_cast<size_type>(sz);
        } else {
            return BUFFER_SIZE - read_index + write_index;
        }
    }

    size_type buffer_size() const
    {
        return BUFFER_SIZE;
    }

    size_type capacity() const
    {
        return BUFFER_SIZE - 1;
    }

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------
    static constexpr size_type inc_index(size_type index)
    {
        return (index + 1) % BUFFER_SIZE;
    }

    // ------------------------
    // PRIVATE: Member data ---
    // ------------------------
    // Storage: raw bytes so T doesn't need to be default-constructible.
    // Elements are placement-constructed on push and destroyed on pop.
    alignas(T) unsigned char m_buffer[BUFFER_SIZE * sizeof(T)];

    // Per-slot ready flag: true when the writer has published the value,
    // false when the slot is empty / has been consumed.
    std::atomic<bool> m_ready[BUFFER_SIZE];

    alignas(ALIGN_SIZE) atomic_size_type m_write_index; // Aligning to avoid "false sharing"
    alignas(ALIGN_SIZE) atomic_size_type m_read_index;  // Aligning to avoid "false sharing"
};

} // END namespace lue
