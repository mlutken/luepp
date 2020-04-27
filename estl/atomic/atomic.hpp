/** @file
 * C++11 atomic operation for use when you don't have C++11
 * standard library available.
 */
#ifndef ESTL_ATOMIC_H
#define ESTL_ATOMIC_H
#include <nestle_default_config.h>
#include <stdbool.h>

namespace estl {

/** C++11 atomic operations for use when you don't have a C++11
 * standard library available.
 * We currently only support integer types. Also we use only __ATOMIC_SEQ_CST mode.
 * @see https://gcc.gnu.org/onlinedocs/gcc-6.1.0/gcc/_005f_005fatomic-Builtins.html
 */
template <class T>
class atomic
{


#if defined(__clang__)
#elif defined(__GNUG__)
#include "atomic__gcc.hpp"
#elif defined(_MSC_VER)

#endif


};


} // END namespace estl

//#if (C_STANDARD == 11)
//#include <stdatomic.h>
//#define cesl_sizeof_atomic_flag sizeof(atomic_flag)
//inline bool cesl_atomic_flag_test_and_set(volatile atomic_flag* obj)
//{
//    return atomic_flag_test_and_set(atomic_flag);
//}

//inline bool cesl_atomic_flag_test_and_set_explicit(volatile atomic_flag* obj, memory_order order)
//{
//    return atomic_flag_test_and_set_explicit(obj, order);
//}

//#elif (C_STANDARD == 99)
//// If complier gcc: TODO: Other compilers
//#define cesl_sizeof_atomic_flag 1

//inline bool cesl_atomic_flag_test_and_set(volatile void* obj)
//{
//    return __atomic_test_and_set (obj, __ATOMIC_SEQ_CST);
//}

//inline bool cesl_atomic_flag_test_and_set_explicit(volatile void* obj, int memory_order)
//{
//    return __atomic_test_and_set (obj, memory_order);
//}


//#define cesl_atomic_store(object, desired) __sync_lock_test_and_set(object, desired)
//#define cesl_atomic_load(object) __sync_fetch_and_add(object, 0 )

//#endif

#endif // ESTL_ATOMIC_H
