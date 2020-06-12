/** @file
 * C++11 atomic operations 'use' include file to allow either
 * internal atomic implementation or to use the compiler supplied one.
 */
#ifndef ESTL_ATOMIC_USE_H
#define ESTL_ATOMIC_USE_H
#include <nestle_default_config.h>

// --- Handle include files ---
#if (CXX_STANDARD_LIBRARY == 98)
#include <atomic/atomic.hpp>
#else
#include <atomic>
#endif

namespace estl_use {

// --- Define correct types depending on configuration ---
#if (CXX_STANDARD_LIBRARY == 98)
template<typename T>
class atomic : public estl::atomic<T>
{
public:
    constexpr atomic()          NESTLE_NOEXEPT : estl::atomic<T>::atomic()  {}
    constexpr atomic(T desired) NESTLE_NOEXEPT : estl::atomic<T>::atomic(desired) {}
    T operator=( T desired ) NESTLE_NOEXEPT { return estl::atomic<T>::operator=(desired); }
    T operator=( T desired ) volatile NESTLE_NOEXEPT { return estl::atomic<T>::operator=(desired); }
};

#else
template<typename T>
using atomic = std::atomic<T>;
#endif

} // END namespace estl_use


#endif // ESTL_ATOMIC_USE_H
