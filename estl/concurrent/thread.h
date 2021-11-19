#ifndef ESTL_THREAD_HPP
#define ESTL_THREAD_HPP
#include <nestle_default_config.h>
#include <cstdint>

#if (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_LINUX)
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_WINDOWS)
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_OSX)
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_FREERTOS)
#include <concurrent/thread__freertos.h>
#else
#   error "Missing estl::mutex system includes !"
#endif // (NESTLE_SYSTEM_NAME)


namespace estl {

class thread
{
public:
    //constexpr mutex() NESTLE_NOEXEPT; // Hmm: How to do this?
	thread() NESTLE_NOEXEPT;
	~thread();

	thread(const mutex& ) = delete;
	thread& operator=(const mutex&) = delete;

private:

};



#endif // ESTL_THREAD_HPP
