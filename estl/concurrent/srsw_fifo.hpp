#ifndef ESTL_SRSW_FIFO_HPP
#define ESTL_SRSW_FIFO_HPP

#include <nestle_default_config.h>
#include <atomic/atomic_use.hpp>
#include <vector>
#include <cstdint>
// #include <align_macros.h> TODO: Use C++11 aligning

// ----------------------------------------------
// --- srsw_lockless_fifo.h ---
// ----------------------------------------------
namespace estl {

template <typename T, class Allocator = std::allocator<T> >
class srsw_fifo
{
private:
    typedef std::vector<T, Allocator >    queue_vec_t;
public:
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    typedef T                                     value_type;
    typedef std::size_t                             size_type;
    typedef Allocator                             allocator_type;
    typedef typename queue_vec_t::difference_type difference_type;
    typedef typename queue_vec_t::reference       reference;
    typedef typename queue_vec_t::const_reference const_reference;
    typedef typename queue_vec_t::pointer         pointer;
    typedef typename queue_vec_t::const_pointer   const_pointer;

    explicit srsw_fifo ( size_type queueSize )
        : m_writeIndex(0)
        , m_queue(queueSize)
        , m_readIndex(0)
    {

    }

    srsw_fifo ( size_type queueSize, const allocator_type& allocator )
        : m_writeIndex(0)
        , m_queue(queueSize, allocator)
        , m_readIndex(0)
    {

    }

    bool  push ( T&& v )
    {
        size_type nextWriteIndex = incIndex(m_writeIndex);
        if ( nextWriteIndex != m_readIndex ) {
            m_queue[m_writeIndex] = std::move(v);
            m_writeIndex = nextWriteIndex;
            return true;
        }
        return false;
    }

    bool  push ( const T& v )
    {
        size_type nextWriteIndex = incIndex(m_writeIndex);
        if ( nextWriteIndex != m_readIndex ) {
            m_queue[m_writeIndex] = v;
            m_writeIndex = nextWriteIndex;
            return true;
        }
        return false;
    }


    bool  pop ()
    {
        if ( empty () ) return false;
        m_readIndex = incIndex (m_readIndex);
        return true;
    }

    T&  front ()
    {
        return m_queue[m_readIndex];
    }

    bool full () const
    {
        size_type writeCheck = incIndex(m_writeIndex);
        return (writeCheck == m_readIndex);
    }

    bool empty () const
    {
        return (m_writeIndex == m_readIndex);
    }

    size_type size() const
    {
        auto sz = static_cast<std::int64_t>(m_writeIndex) - static_cast<std::int64_t>(m_readIndex);
        if (sz >= 0) {
            return static_cast<size_type>(sz);
        }
        else {
            return static_cast<size_type>(-sz -1);
        }
    }

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------
    size_type    incIndex ( size_type index ) const { return (index +1) % m_queue.size(); }

    // ------------------------
    // PRIVATE: Member data ---
    // ------------------------
    using aligned_size_type_t = std::size_t; // FIXMENM Use C++11 align
//    _ALIGNED_TYPE(size_type, 64) aligned_size_type_t;
    // TODO: Use C++11 aligning 
    volatile aligned_size_type_t  m_writeIndex;   // Aligning to avoid "false sharing"
    queue_vec_t                   m_queue;
    volatile aligned_size_type_t  m_readIndex;    // Aligning to avoid "false sharing"

};

} // END namespace estl

#endif // ESTL_SRSW_FIFO_HPP
