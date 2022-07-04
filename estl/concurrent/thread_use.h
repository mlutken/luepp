/** @file
 * C++11 thread classes 'use' include file to allow either
 * internal thread implementation or to use the compiler supplied one.
 */
#ifndef ESTL_THREAD_USE_H
#define ESTL_THREAD_USE_H
#include <luepp_platform_definitions.h>
#include <luepp_default_config.h>

// --- Handle include files ---
#if (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_FREERTOS)
#include <concurrent/thread.hpp>
#else
#include <thread>
#endif

namespace estl_use {

// --- Define correct types depending on configuration ---
#if (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_FREERTOS)
using thread = estl::thread;
#else
using thread = std::thread;
#endif

} // END namespace estl_use


#endif // ESTL_THREAD_USE_H
