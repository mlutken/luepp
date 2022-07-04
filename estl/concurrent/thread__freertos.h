#ifndef ESTL_THREAD_FREERTOS_HPP
#define ESTL_THREAD_FREERTOS_HPP
#include <luepp_default_config.h>
#include <cstdint>

#if (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_LINUX)
#include <pthread.h>
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_WINDOWS)
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_OSX)
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_FREERTOS)
#else
#   error "Missing estl::mutex system includes !"
#endif // (LUEPP_SYSTEM_ID)


namespace estl {

class thread
{
public:
    //constexpr mutex() noexcept; // Hmm: How to do this?
	thread() noexcept;
	~thread();

	thread(const mutex& ) = delete;
	thread& operator=(const mutex&) = delete;

private:

};



#endif // ESTL_THREAD_FREERTOS_HPP
