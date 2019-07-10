/**@file
 * cesl_align_macros Atomic functions for pre C11/C++11 code.
 * Modelled after the C11/C++11 atomic.h functions.
 */
#ifndef CESL_COMPILER_ATOMIC_H
#define CESL_COMPILER_ATOMIC_H
#include <stdbool.h>

#if (C_STANDARD == 11)
#include <stdatomic.h>
#define cesl_sizeof_atomic_flag sizeof(atomic_flag)
inline bool cesl_atomic_flag_test_and_set(volatile atomic_flag* obj)
{
    return atomic_flag_test_and_set(atomic_flag);
}

inline bool cesl_atomic_flag_test_and_set_explicit(volatile atomic_flag* obj, memory_order order)
{
    return atomic_flag_test_and_set_explicit(obj, order);
}

#elif (C_STANDARD == 99)
// If complier gcc: TODO: Other compilers
#define cesl_sizeof_atomic_flag 1

inline bool cesl_atomic_flag_test_and_set(volatile void* obj)
{
    return __atomic_test_and_set (obj, __ATOMIC_SEQ_CST);
}

inline bool cesl_atomic_flag_test_and_set_explicit(volatile void* obj, int memory_order)
{
    return __atomic_test_and_set (obj, memory_order);
}


#define cesl_atomic_store(object, desired) __sync_lock_test_and_set(object, desired)
#define cesl_atomic_load(object) __sync_fetch_and_add(object, 0 )

#endif

#endif // CESL_COMPILER_ATOMIC_H
