#ifndef ESTL_SRSW_FIFO_HPP
#define ESTL_SRSW_FIFO_HPP

#include <luepp_platform_definitions.h>
#include <luepp_default_config.h>
#include <atomic/atomic_use.hpp>
#include <vector>
#include <cstdint>

// ----------------------------------------------
// --- srsw_fifo.h ---
// ----------------------------------------------
namespace estl {

/**
Single reader, single writer lockless fifo.
Uses atomics for the read and write indices internally.
@sa estl::srsw_fifo_s which is the same using the static vector_s as "backend"
*/

template <typename T, class Allocator = std::allocator<T>, size_t ALIGN_SIZE = 128 >
class srsw_fifo
{
private:
    typedef std::vector<T, Allocator >    queue_vec_t;
public:
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    typedef T                                       value_type;
    typedef std::size_t                             size_type;
    typedef estl_use::atomic<size_type>             atomic_size_type;
    typedef Allocator                               allocator_type;
    typedef typename queue_vec_t::difference_type   difference_type;
    typedef typename queue_vec_t::reference         reference;
    typedef typename queue_vec_t::const_reference   const_reference;
    typedef typename queue_vec_t::pointer           pointer;
    typedef typename queue_vec_t::const_pointer     const_pointer;

    srsw_fifo ()
        : m_write_index(0)
        , m_queue(0)
        , m_read_index(0)
    {
    }

    explicit srsw_fifo (size_type queueSize)
        : m_write_index(0)
        , m_queue(queueSize)
        , m_read_index(0)
    {
    }

    srsw_fifo (size_type queueSize, const allocator_type& allocator)
        : m_write_index(0)
        , m_queue(queueSize, allocator)
        , m_read_index(0)
    {
    }

    void init_queue(size_type queue_size) {
        m_queue.reserve(queue_size);
        for (size_t n = 0; n != queue_size; ++n) {
            m_queue.push_back(value_type{});
        }
        m_write_index = 0;
        m_read_index = 0;
    }

    void resize_queue(size_type queue_size) {
        queue_vec_t temp;
        temp.reserve(buffer_size());
        while (!empty()) {
            temp.push_back(std::move(front()));
            pop();
        }

        init_queue(queue_size);
        for (auto&& elem: temp) {
            push(std::move(elem));
        }
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
        return m_queue.size();
    }

    size_type capacity() const
    {
        return m_queue.size();
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
    alignas(ALIGN_SIZE) atomic_size_type    m_write_index;   // Aligning to avoid "false sharing"
    queue_vec_t                             m_queue;
    alignas(ALIGN_SIZE) atomic_size_type    m_read_index;    // Aligning to avoid "false sharing"
};

} // END namespace estl

#endif // ESTL_SRSW_FIFO_HPP
