#ifndef ESTL_MUTEX_HPP
#define ESTL_MUTEX_HPP
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
class mutex
{
public:

    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
    mutex();
#if (CXX_STANDARD != 98)
    mutex(const mutex& other) = delete;
    mutex& operator=(const mutex&) = delete;
#else
private:
    mutex(const atomic&);
    atomic& operator=(const atomic&);
public:
#endif

    // ------------------------
    // --- Helper functions ---
    // ------------------------
private:

    // --- Member data --
};

#if (CXX_STANDARD != 98)
//#   include "mutex__cpp11.hpp"
#endif

// ------------------------
// -- Compare operators ---
// ------------------------

} // END namespace estl


template <class MUTEX>
class lock_guard
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
//#   include "lock_guard__cpp11.hpp"
#endif
};

// ------------------------
// -- Compare operators ---
// ------------------------

} // END namespace estl



#endif // ESTL_MUTEX_HPP
