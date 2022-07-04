/** @file
 * C++11 atomic operation for use when you don't have C++11
 * standard library available.
 */
#ifndef ESTL_ATOMIC_H
#define ESTL_ATOMIC_H
#include <luepp_platform_definitions.h>
#include <luepp_default_config.h>
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
    constexpr atomic() noexcept;
    constexpr atomic(T desired) noexcept;

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

    T operator=( T desired ) noexcept;
    T operator=( T desired ) volatile noexcept;

    operator T() const noexcept;
    operator T() const volatile noexcept;

    // Pre increment
    T operator++() noexcept;
    T operator++() volatile noexcept;

    // Post increment
    T operator++(int) noexcept;
    T operator++(int) volatile noexcept;

    // Pre decrement
    T operator--() noexcept;
    T operator--() volatile noexcept;

    // Post decrement
    T operator--(int) noexcept;
    T operator--(int) volatile noexcept;

    T operator+=(T arg) noexcept;
    T operator+=(T arg) volatile noexcept;

    T operator-=(T arg) noexcept;
    T operator-=(T arg) volatile noexcept;

    T operator&=(T arg) noexcept;
    T operator&=(T arg) volatile noexcept;

    T operator|=(T arg) noexcept;
    T operator|=(T arg) volatile noexcept;

    T operator^=(T arg) noexcept;
    T operator^=(T arg) volatile noexcept;

    bool is_lock_free   () const noexcept;
    bool is_lock_free   () const volatile noexcept;
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
