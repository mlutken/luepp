#pragma once

#include <luepp_default_config.h>
#include <atomic>
#include <vector>
#include <cstdint>

// ----------------------------------------------
// --- mrmw_fifo.h ---
// ----------------------------------------------
namespace lue {

/**
Multiple reader, multiple writer lockless fifo.
Uses atomics for the read and write indices internally and for any extra
bookkeeping needed for lock-free operation.
*/

template <typename T, class Allocator = std::allocator<T>, size_t ALIGN_SIZE = 128>
class mrmw_fifo
{
private:
    typedef std::vector<T, Allocator>    queue_vec_t;

public:
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    typedef T                                       value_type;
    typedef std::size_t                             size_type;
    typedef std::atomic<size_type>                  atomic_size_type;
    typedef Allocator                               allocator_type;
    typedef typename queue_vec_t::difference_type   difference_type;
    typedef typename queue_vec_t::reference         reference;
    typedef typename queue_vec_t::const_reference   const_reference;
    typedef typename queue_vec_t::pointer           pointer;
    typedef typename queue_vec_t::const_pointer     const_pointer;

    mrmw_fifo()
        : m_head(0)
        , m_queue(0)
        , m_tail(0)
    {
    }

    explicit mrmw_fifo(size_type queue_size)
        : m_head(0)
        , m_queue(queue_size)
        , m_tail(0)
    {
    }

    mrmw_fifo(size_type queue_size, const allocator_type& allocator)
        : m_head(0)
        , m_queue(queue_size, allocator)
        , m_tail(0)
    {
    }

    void init_queue(size_type queue_size)
    {
        m_queue.clear();
        m_queue.reserve(queue_size);
        for (size_type n = 0; n != queue_size; ++n) {
            m_queue.push_back(value_type{});
        }
        m_head.store(0, std::memory_order_relaxed);
        m_tail.store(0, std::memory_order_relaxed);
    }

    void resize_queue(size_type queue_size)
    {
        queue_vec_t temp;
        temp.reserve(buffer_size());
        while (!empty()) {
            temp.push_back(std::move(front()));
            pop();
        }

        init_queue(queue_size);
        for (auto&& elem : temp) {
            push(std::move(elem));
        }
    }

    bool push(T&& v)
    {
        size_type tail = m_tail.load(std::memory_order_relaxed);
        size_type head = m_head.load(std::memory_order_acquire);

        while (true) {
            size_type next_tail = inc_index(tail);
            if (next_tail == head) {
                return false; // full
            }
            if (m_tail.compare_exchange_weak(
                    tail, next_tail, std::memory_order_seq_cst, std::memory_order_relaxed)) {
                m_queue[tail] = std::move(v);
                return true;
            }
        }
    }

    template <class... Args>
    bool emplace(Args&&... args)
    {
        size_type tail = m_tail.load(std::memory_order_relaxed);
        size_type head = m_head.load(std::memory_order_acquire);

        while (true) {
            size_type next_tail = inc_index(tail);
            if (next_tail == head) {
                return false; // full
            }
            if (m_tail.compare_exchange_weak(
                    tail, next_tail, std::memory_order_seq_cst, std::memory_order_relaxed)) {
                new (&m_queue[tail]) value_type(std::forward<Args>(args)...);
                return true;
            }
        }
    }

    bool push(const T& v)
    {
        size_type tail = m_tail.load(std::memory_order_relaxed);
        size_type head = m_head.load(std::memory_order_acquire);

        while (true) {
            size_type next_tail = inc_index(tail);
            if (next_tail == head) {
                return false; // full
            }
            if (m_tail.compare_exchange_weak(
                    tail, next_tail, std::memory_order_seq_cst, std::memory_order_relaxed)) {
                m_queue[tail] = v;
                return true;
            }
        }
    }

    bool pop()
    {
        size_type head = m_head.load(std::memory_order_relaxed);
        size_type tail = m_tail.load(std::memory_order_acquire);

        while (true) {
            if (head == tail) {
                return false; // empty
            }
            if (m_head.compare_exchange_weak(
                    head, inc_index(head), std::memory_order_seq_cst, std::memory_order_relaxed)) {
                return true;
            }
        }
    }

    T& front()
    {
        return m_queue[m_head.load(std::memory_order_acquire)];
    }

    const T& front() const
    {
        return m_queue[m_head.load(std::memory_order_acquire)];
    }

    void flush()
    {
        auto elements_to_pop = size();
        while (!empty() && elements_to_pop > 0) {
            pop();
            --elements_to_pop;
        }
    }

    bool full() const
    {
        size_type head = m_head.load(std::memory_order_acquire);
        size_type tail = m_tail.load(std::memory_order_acquire);
        return inc_index(tail) == head;
    }

    bool empty() const
    {
        return m_head.load(std::memory_order_acquire) == m_tail.load(std::memory_order_acquire);
    }

    size_type size() const
    {
        const size_type head = m_head.load(std::memory_order_acquire);
        const size_type tail = m_tail.load(std::memory_order_acquire);
        std::int64_t sz = static_cast<std::int64_t>(tail) - static_cast<std::int64_t>(head);
        if (sz >= 0) {
            return static_cast<size_type>(sz);
        } else {
            return buffer_size() - head + tail;
        }
    }

    size_type buffer_size() const
    {
        return m_queue.size();
    }

    size_type capacity() const
    {
        // For circular buffer: capacity = buffer_size - 1
        // because one slot is reserved to distinguish full from empty
        size_type buf_size = buffer_size();
        return buf_size > 0 ? buf_size - 1 : 0;
    }

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------
    size_type inc_index(size_type index) const
    {
        return (index + 1) % buffer_size();
    }

    // ------------------------
    // PRIVATE: Member data ---
    // ------------------------
    alignas(ALIGN_SIZE) atomic_size_type m_head;
    queue_vec_t m_queue;
    alignas(ALIGN_SIZE) atomic_size_type m_tail;
};

} // END namespace lue
