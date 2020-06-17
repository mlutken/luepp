#ifndef ESTL_MUTEX_HPP
#define ESTL_MUTEX_HPP
#include <nestle_default_config.h>
#include <cstdint>

#if (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_LINUX)
#include <pthread.h>
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_WINDOWS)
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_OSX)
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_FREERTOS)
#else
#   error "Missing estl::mutex system includes !"
#endif // (NESTLE_SYSTEM_NAME)

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
    //constexpr mutex() NESTLE_NOEXEPT; // Hmm: How to do this?
    mutex() NESTLE_NOEXEPT;
    ~mutex();

#if (CXX_STANDARD != 98)
    mutex(const mutex& ) = delete;
    mutex& operator=(const mutex&) = delete;
#else
private:
    mutex(const mutex&);
    mutex& operator=(const mutex&);
public:
#endif // (CXX_STANDARD != 98)

    void lock();
    bool try_lock();
    void unlock();
//    native_handle_type native_handle();

private:
#if (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_LINUX)
    pthread_mutex_t pthread_mutex_;
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_WINDOWS)
#   error "Missing estl::mutex Windows header member data !"
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_OSX)
#   error "Missing estl::mutex OSX header member data !"
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_FREERTOS)
    void ensure_created();
    SemaphoreHandle_t semaphore_ = nullptr;
#else
#   error "Missing estl::mutex system header member data !"
#endif // (NESTLE_SYSTEM_NAME)

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

#if (CXX_STANDARD != 98)
    lock_guard (const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
#else
private:
    lock_guard (const lock_guard&);
    lock_guard& operator=(const lock_guard&);
public:
#endif // (CXX_STANDARD != 98)
private:
    mutex_type& mutex_;
};

} // END namespace estl



#endif // ESTL_MUTEX_HPP
