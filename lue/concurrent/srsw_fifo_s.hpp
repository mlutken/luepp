#pragma once

#include <luepp_default_config.h>
#include <atomic>
#include <containers/vector_s.hpp>
#include <cstdint>

// ----------------------------------------------
// --- srsw_fifo_s.h ---
// ----------------------------------------------
namespace lue {

template <typename T1, size_t BUFFER_SIZE1, size_t ALIGN_SIZE1 >
class srmw_fifo_s;

/**
 * @brief Ringbuffer lockless FIFO template with one reader and one writer.
 * Single reader, single writer lockless fifo (ring buffer).
 * Uses atomics for the read and write indices internally and uses vector_s
 * as storage container.
 * @sa lue::srsw_fifo which is the same using std::vector as "backend"
 * @example

#include <iostream>
#include <concurrent/srsw_fifo_s.hpp>

using namespace lue;

void fifo_example()
{
    using my_fifo = srsw_fifo_s<int,12>;
    my_fifo f;

    std::cerr << "Initial f.size(): " << f.size() << "\n";
    f.push(10); // size = 1
    f.push(20); // size = 2
    f.push(30); // size = 3
    std::cerr << "f.front(): " << f.front() << "\n";  // Outputs 10
    f.pop();    // size = 2
    f.push(40); // size = 3
    const auto sz = f.size();

    std::cerr << "f.size(): " << sz << "\n";
}

 * @endexample
 
*/

template <typename T, size_t BUFFER_SIZE, size_t ALIGN_SIZE = 128 >
class srsw_fifo_s
{
private:
    template <typename T1, size_t BUFFER_SIZE1, size_t ALIGN_SIZE1>
    friend class srmw_fifo_s;

    typedef lue::vector_s<T, BUFFER_SIZE>    queue_vec_t;
public:
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    typedef T                                       value_type;
    typedef std::size_t                             size_type;
    typedef std::atomic<size_type>                  atomic_size_type;
    typedef typename queue_vec_t::difference_type   difference_type;
    typedef typename queue_vec_t::reference         reference;
    typedef typename queue_vec_t::const_reference   const_reference;
    typedef typename queue_vec_t::pointer           pointer;
    typedef typename queue_vec_t::const_pointer     const_pointer;

    srsw_fifo_s ()
        : m_write_index(0)
        , m_read_index(0)
    {
    }

    bool push (T&& v)
    {
        const size_type write_index = m_write_index;
        const size_type next_write_index = incIndex(write_index);
        if ( next_write_index != m_read_index ) {
            m_queue[write_index] = std::move(v);
            m_write_index = next_write_index;
            return true;
        }
        return false;
    }

    bool push (const T& v)
    {
        const size_type write_index = m_write_index;
        const size_type next_write_index = incIndex(write_index);
        if ( next_write_index != m_read_index ) {
            m_queue[write_index] = v;
            m_write_index = next_write_index;
            return true;
        }
        return false;
    }

    template<class... Args >
    bool emplace(Args&&... args)
    {
        const size_type write_index = m_write_index;
        const size_type next_write_index = incIndex(write_index);
        if ( next_write_index != m_read_index ) {
            const pointer insert_ptr = &m_queue[write_index];
            new (insert_ptr) value_type(std::forward<Args>(args)...);
            m_write_index = next_write_index;
            return true;
        }
        return false;
    }

    bool pop ()
    {
        if ( empty () ) return false;
        m_read_index = incIndex (m_read_index);
        return true;
    }

    T& front ()
    {
        return m_queue[m_read_index];
    }

    const T& front () const
    {
        return m_queue[m_read_index];
    }

    void flush () {
        auto elements_to_pop = size();
        while (!empty() && elements_to_pop > 0 ) {
            pop();
            --elements_to_pop;
        }
    }

    bool full () const
    {
        size_type writeCheck = incIndex(m_write_index);
        return (writeCheck == m_read_index);
    }

    bool empty () const
    {
        return (m_write_index == m_read_index);
    }

    size_type size() const
    {
        const size_type read_index = m_read_index;
        const size_type write_index = m_write_index;
        std::int64_t sz = static_cast<std::int64_t>(write_index) - static_cast<std::int64_t>(read_index);
        if (sz >= 0) {
            return static_cast<size_type>(sz);
        }
        else {
            return buffer_size() - read_index + write_index;
        }
    }

    size_type buffer_size() const
    {
        return BUFFER_SIZE;
    }

    size_type capacity() const
    {
        return BUFFER_SIZE;
    }

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------
    const T* reserve ()
    {
        const size_type write_index = m_write_index;
        const size_type next_write_index = incIndex(write_index);
        if ( next_write_index != m_read_index ) {
            m_write_index = next_write_index;
            const T* ptr = &(m_queue[write_index]);
            return ptr;
        }
        return nullptr;
    }

    size_type    incIndex ( size_type index ) const
    {
        return (index +1) % buffer_size();
    }

    // ------------------------
    // PRIVATE: Member data ---
    // ------------------------
    alignas(ALIGN_SIZE) atomic_size_type    m_write_index;   // Aligning to avoid "false sharing"
    queue_vec_t                             m_queue;
    alignas(ALIGN_SIZE) atomic_size_type    m_read_index;    // Aligning to avoid "false sharing"
};

} // END namespace lue
