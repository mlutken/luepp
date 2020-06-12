#ifndef ESTL_SRSW_FIFO_S_HPP
#define ESTL_SRSW_FIFO_S_HPP

#include <nestle_default_config.h>
#include <atomic/atomic_use.hpp>
#include <containers/vector_s.hpp>
#include <cstdint>

// ----------------------------------------------
// --- srsw_fifo_s.h ---
// ----------------------------------------------
namespace estl {

/**
Single reader, single writer lockless fifo.
Uses atomics for the read and write indices internally.
@sa estl::srsw_fifo which is the same using std::vector as "backend"
*/

template <typename T, size_t BUFFER_SIZE, size_t ALIGN_SIZE = 128 >
class srsw_fifo_s
{
private:
    typedef estl::vector_s<T, BUFFER_SIZE>    queue_vec_t;
public:
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    typedef T                                       value_type;
    typedef std::size_t                             size_type;
    typedef estl_use::atomic<size_type>             atomic_size_type;
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
            m_queue[write_index] = NESTLE_MOVE(v);
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

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------
    size_type    incIndex ( size_type index ) const
    {
        return (index +1) % buffer_size();
    }

    // ------------------------
    // PRIVATE: Member data ---
    // ------------------------
    NESTLE_ALIGNAS(ALIGN_SIZE) atomic_size_type    m_write_index;   // Aligning to avoid "false sharing"
    queue_vec_t                                    m_queue;
    NESTLE_ALIGNAS(ALIGN_SIZE) atomic_size_type    m_read_index;    // Aligning to avoid "false sharing"
};

} // END namespace estl

#endif // ESTL_SRSW_FIFO_S_HPP
