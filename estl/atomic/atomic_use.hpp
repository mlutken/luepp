/** @file
 * C++11 atomic operations 'use' include file to allow either
 * internal atomic implementation or to use the compiler supplied one.
 */
#ifndef ESTL_ATOMIC_USE_H
#define ESTL_ATOMIC_USE_H
#include <luepp_platform_definitions.h>
#include <luepp_default_config.h>

// --- Handle include files ---
#if (LUEPP_HAS_ATOMIC == 0)
#include <atomic/atomic.hpp>
#else
#include <atomic>
#endif

namespace estl_use {

// --- Define correct types depending on configuration ---
#if (LUEPP_HAS_ATOMIC == 0)
template<typename T>
class atomic : public estl::atomic<T>
{
public:
    constexpr atomic()          noexcept : estl::atomic<T>::atomic()  {}
    constexpr atomic(T desired) noexcept : estl::atomic<T>::atomic(desired) {}
    T operator=( T desired ) noexcept { return estl::atomic<T>::operator=(desired); }
    T operator=( T desired ) volatile noexcept { return estl::atomic<T>::operator=(desired); }
};

#else
template<typename T>
using atomic = std::atomic<T>;
#endif

} // END namespace estl_use


#endif // ESTL_ATOMIC_USE_H
