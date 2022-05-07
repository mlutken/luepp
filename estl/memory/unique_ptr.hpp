#ifndef ESTL_UNIQUE_PTR_HPP
#define ESTL_UNIQUE_PTR_HPP
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
template <typename T>
class unique_ptr
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

#if (CXX_STANDARD != 98)
//#   include "unique_ptr__cpp11.hpp"
#endif
};

// ------------------------
// -- Compare operators ---
// ------------------------

} // END namespace estl



#endif // ESTL_UNIQUE_PTR_HPP
