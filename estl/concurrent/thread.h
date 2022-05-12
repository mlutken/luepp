#ifndef ESTL_THREAD_HPP
#define ESTL_THREAD_HPP
#include <luepp_default_config.h>
#include <cstdint>

#if (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_LINUX)
#elif (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_WINDOWS)
#elif (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_OSX)
#elif (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_FREERTOS)
#include <concurrent/thread__freertos.h>
#else
#   error "Missing estl::mutex system includes !"
#endif // (LUEPP_SYSTEM_NAME)


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



#endif // ESTL_THREAD_HPP
