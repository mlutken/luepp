/** @file
 * C++11 mutex classes 'use' include file to allow either
 * internal mutex implementation or to use the compiler supplied one.
 */
#ifndef ESTL_MUTEX_USE_H
#define ESTL_MUTEX_USE_H
#include <luepp_default_config.h>

// --- Handle include files ---
#if (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_FREERTOS)
#include <concurrent/mutex.hpp>
#else
#include <mutex>
#endif

namespace estl_use {

// --- Define correct types depending on configuration ---
#if (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_FREERTOS)
using mutex = estl::mutex;
#else
using mutex = std::mutex;
#endif

} // END namespace estl_use


#endif // ESTL_MUTEX_USE_H
