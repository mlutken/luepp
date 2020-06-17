/** @file
 * C++11 mutex classes 'use' include file to allow either
 * internal mutex implementation or to use the compiler supplied one.
 */
#ifndef ESTL_MUTEX_USE_H
#define ESTL_MUTEX_USE_H
#include <nestle_default_config.h>

// --- Handle include files ---
#if (CXX_STANDARD_LIBRARY == 98)
#include <concurrent/mutex.hpp>
#else
#include <mutex>
#endif

namespace estl_use {

// --- Define correct types depending on configuration ---
#if (CXX_STANDARD_LIBRARY == 98)
typedef estl::mutex mutex;
#else
using mutex = std::mutex;
#endif

} // END namespace estl_use


#endif // ESTL_MUTEX_USE_H
