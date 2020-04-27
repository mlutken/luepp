#ifndef ESTL_BITSET_HPP
#define ESTL_BITSET_HPP
#include <nestle_default_config.h>
#include <cstdint>
// #include <utility>
// #include <iterator>
// #include <stdexcept>
// #include <algorithm>


namespace estl {
/** 
 
@todo NOT IMPLEMENTED AT ALL YET!!! 
 */
template <typename T, size_t CAPACITY>
class bitset
{
public:

    typedef T                                           value_type;
    typedef size_t                                      size_type;
    typedef ptrdiff_t                                   difference_type;
    typedef value_type&                                 reference;
    typedef const value_type&                           const_reference;
    typedef value_type*                                 pointer;
    typedef const value_type*                           const_pointer;
    typedef value_type*                                 iterator;
    typedef const value_type*                           const_iterator;
    typedef std::reverse_iterator<iterator>             reverse_iterator;
    typedef const std::reverse_iterator<const_iterator> const_reverse_iterator;

    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
    // ----------------------
    // --- Element access ---
    // ----------------------

    // ----------------
    // --- Capacity ---
    // ----------------
    bool          empty       () const NESTLE_NOEXEPT {   return size_ == 0;    }
    size_type     size        () const NESTLE_NOEXEPT {   return size_;         }
    size_type     max_size    () const NESTLE_NOEXEPT {   return CAPACITY;      }
    size_type     capacity    () const NESTLE_NOEXEPT {   return CAPACITY;      }

    // -----------------
    // --- Modifiers ---
    // -----------------
    // ------------------------
    // --- Helper functions ---
    // ------------------------
private:

private:
    // --- Member data --
    // --- Implementation remark: ---
    // We do not use the simpler 'value_type  data_[CAPACITY];'
    // data memeber since that would default initialize the entire array, which
    // we don't want to spend time doing as the container conceptually is empty
    // after construction, unless we are using one of the  initializing constructors.
    // So: We simply want to reserve the memory at first.
    size_type       size_ = 0u;
    char            data_[CAPACITY*sizeof(value_type)]; // TODO: Use std::byte when we require C++17
    pointer         data_ptr_ = reinterpret_cast<pointer>(&data_[0]);

#if (CXX_STANDARD != 98)
//#   include "bitset__cpp11.hpp"
#endif
};

// ------------------------
// -- Compare operators ---
// ------------------------

} // END namespace estl

#endif // ESTL_BITSET_HPP
