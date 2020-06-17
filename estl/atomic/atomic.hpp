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
public:
    constexpr atomic() NESTLE_NOEXEPT;
    constexpr atomic(T desired) NESTLE_NOEXEPT;

#if (CXX_STANDARD != 98)
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
#else
private:
    atomic(const atomic&);
    atomic& operator=(const atomic&);
    atomic& operator=(const atomic&) volatile;
public:
#endif

    T operator=( T desired ) NESTLE_NOEXEPT;
    T operator=( T desired ) volatile NESTLE_NOEXEPT;

    operator T() const NESTLE_NOEXEPT;
    operator T() const volatile NESTLE_NOEXEPT;

    // Pre increment
    T operator++() NESTLE_NOEXEPT;
    T operator++() volatile NESTLE_NOEXEPT;

    // Post increment
    T operator++(int) NESTLE_NOEXEPT;
    T operator++(int) volatile NESTLE_NOEXEPT;

    // Pre decrement
    T operator--() NESTLE_NOEXEPT;
    T operator--() volatile NESTLE_NOEXEPT;

    // Post decrement
    T operator--(int) NESTLE_NOEXEPT;
    T operator--(int) volatile NESTLE_NOEXEPT;

    T operator+=(T arg) NESTLE_NOEXEPT;
    T operator+=(T arg) volatile NESTLE_NOEXEPT;

    T operator-=(T arg) NESTLE_NOEXEPT;
    T operator-=(T arg) volatile NESTLE_NOEXEPT;

    T operator&=(T arg) NESTLE_NOEXEPT;
    T operator&=(T arg) volatile NESTLE_NOEXEPT;

    T operator|=(T arg) NESTLE_NOEXEPT;
    T operator|=(T arg) volatile NESTLE_NOEXEPT;

    T operator^=(T arg) NESTLE_NOEXEPT;
    T operator^=(T arg) volatile NESTLE_NOEXEPT;

    bool is_lock_free   () const NESTLE_NOEXEPT;
    bool is_lock_free   () const volatile NESTLE_NOEXEPT;
private:
    // If needed for a given platform/compiler add #ifdefs here!
    volatile mutable T value_;
};



} // END namespace estl

#if defined(__clang__)
    #include "atomic__clang.hpp"
#elif defined(__GNUG__)
    #include "atomic__gcc.hpp"
#elif defined(_MSC_VER)
#endif

#endif // ESTL_ATOMIC_H
