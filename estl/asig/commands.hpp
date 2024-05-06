#ifndef ESTL_MUTEX_HPP
#define ESTL_MUTEX_HPP
#include <luepp_platform_definitions.h>
#include <luepp_default_config.h>
#include <cstdint>

#if (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_LINUX)
#include <pthread.h>
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_WINDOWS)
#include <pthread.h>
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_OSX)
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_FREERTOS)
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_BROWSER)
#include <pthread.h>
#else
#   error "Missing estl::mutex system includes !"
#endif // (LUEPP_SYSTEM_ID)

// -------------------
// --- mutex, lock ---
// -------------------

namespace estl {
struct defer_lock_t { };
struct try_to_lock_t { };
struct adopt_lock_t { };

class mutex
{
public:
    //constexpr mutex() noexcept; // Hmm: How to do this?
    mutex() noexcept;
    ~mutex();

    mutex(const mutex& ) = delete;
    mutex& operator=(const mutex&) = delete;

    void lock();
    bool try_lock();
    void unlock();
//    native_handle_type native_handle();

private:
#if (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_LINUX)
    pthread_mutex_t pthread_mutex_;
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_WINDOWS)
    pthread_mutex_t pthread_mutex_;
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_OSX)
#   error "Missing estl::mutex OSX header member data !"
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_FREERTOS)
    void ensure_created();
    SemaphoreHandle_t semaphore_ = nullptr;
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_BROWSER)
    pthread_mutex_t pthread_mutex_;
#else
#   error "Missing estl::mutex system header member data !"
#endif // (LUEPP_SYSTEM_ID)

};

template <class MUTEX>
class lock_guard {
 public:
    typedef MUTEX mutex_type;
    explicit lock_guard(mutex_type& m)
        : mutex_(m)
    {
        mutex_.lock();
    }

    lock_guard(mutex_type& m, adopt_lock_t)
        : mutex_(m)
    {
    }

    ~lock_guard()
    {
        mutex_.unlock();
    }

    lock_guard (const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
private:
    mutex_type& mutex_;
};

} // END namespace estl



#endif // ESTL_MUTEX_HPP
