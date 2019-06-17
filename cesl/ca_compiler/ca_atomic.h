/**@file
 * cesl_align_macros Atomic functions for pre C11/C++11 code.
 * Modelled after the C11/C++11 atomic.h functions.
 */
#ifndef CESL_COMPILER_ATOMIC_H
#define CESL_COMPILER_ATOMIC_H

#define cesl_atomic_store(object, desired) __sync_lock_test_and_set(object, desired)
#define cesl_atomic_load(object) __sync_fetch_and_add(object, 0 )

#endif // CESL_COMPILER_ATOMIC_H
