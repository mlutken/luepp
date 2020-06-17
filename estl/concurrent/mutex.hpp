#ifndef ESTL_MUTEX_HPP
#define ESTL_MUTEX_HPP
#include <nestle_default_config.h>
#include <cstdint>

#if (CXX_STANDARD_LIBRARY == 98)

// -------------------
// --- mutex, lock ---
// -------------------

namespace estl {
struct defer_lock_t { };
struct try_to_lock_t { };
struct adopt_lock_t { };

/*
class mutex {
 public:
    constexpr mutex() noexcept;
    ~mutex();
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    void lock();
    bool try_lock();
    void unlock();
//    typedef implementation-defined native_handle_type;
    native_handle_type native_handle();
};
*/


//#define USER "jack" // jack or queen
#define USER 'j' // jack or queen

//#if USER == 'j'
//#error "User is jack"
//#else
//#error "User is NOT Jack"
//#endif

//#if (NESTLE_PLATFORM_NAME == "linux")
//#error "NESTLE_PLATFORM_NAME 'linux'"
//#else
//#error "NESTLE_PLATFORM_NAME 'unknown'"
//#endif

class mutex
{
public:
    constexpr mutex() NESTLE_NOEXEPT;

#if (CXX_STANDARD != 98)
    mutex(const mutex& ) = delete;
    //mutex& operator=(const mutex&) = delete;
#else
private:
    mutex(const mutex&);
    //mutex& operator=(const mutex&);
public:
#endif

    void lock();
    bool try_lock();
    void unlock();
//    native_handle_type native_handle();
};



#endif // (CXX_STANDARD_LIBRARY == 98)

/*
FreeRTOS Mutex

class Mutex {
public:
    Mutex() = default;
    ~Mutex() { vSemaphoreDelete(xSemaphore); }

    inline bool init();
    inline void lock();
    inline void unlock();

private:
    SemaphoreHandle_t xSemaphore = NULL;
};
template <class Mutex>
class lock_guard {
public:
    typedef Mutex           mutex_type;

    explicit lock_guard( mutex_type& m );
    lock_guard( mutex_type& m, estl::adopt_lock_t t );
    lock_guard( const lock_guard& ) = delete;

    LockGuard(Mutex& mutex)
        : mtx(mutex)
        { mtx.lock(); }

    ~LockGuard() { mtx.unlock(); }

    // Inhibit copy and move of the LockGuard by ctor or operator=
    LockGuard(const LockGuard&)              = delete;
    LockGuard& operator=(const LockGuard&)   = delete;
    LockGuard(const LockGuard&&)             = delete;
    LockGuard& operator=(const LockGuard&&)  = delete;
private:
    T& mtx;
};

inline void Mutex::lock()
{
    bool valid_mtx = init(); // Init on first lock
    MODI_ASSERT(valid_mtx, EX_APP_MODULE_DATA_NOT_INITIALIZED, AUDIOROUTER);

    xSemaphoreTake(xSemaphore, portMAX_DELAY);
}

inline void Mutex::unlock()
{
    xSemaphoreGive(xSemaphore);
}

inline bool Mutex::init()
{
    if (NULL != xSemaphore)
        return true;
    else
    {
        xSemaphore = xSemaphoreCreateMutex();
        return (NULL != xSemaphore);
    }
    return false;
}

*/

/** 
 
@todo NOT IMPLEMENTED AT ALL YET!!! 
 */

#if (CXX_STANDARD != 98)
//#   include "mutex__cpp11.hpp"
#endif


template <class MUTEX>
class lock_guard {
 public:
    typedef MUTEX mutex_type;
    explicit lock_guard(mutex_type& m);
    lock_guard(mutex_type& m, adopt_lock_t);
    ~lock_guard();
    lock_guard(lock_guard const&) = delete;
    lock_guard& operator=(lock_guard const&) = delete;
 private:
    mutex_type& pm;
};

//template <class MUTEX>
//class lock_guard
//{
//public:

//    typedef T                                           value_type;
//    typedef size_t                                      size_type;
//    typedef ptrdiff_t                                   difference_type;
//    typedef value_type&                                 reference;
//    typedef const value_type&                           const_reference;
//    typedef value_type*                                 pointer;
//    typedef const value_type*                           const_pointer;
//    typedef value_type*                                 iterator;
//    typedef const value_type*                           const_iterator;
//    typedef std::reverse_iterator<iterator>             reverse_iterator;
//    typedef const std::reverse_iterator<const_iterator> const_reverse_iterator;

//    // ------------------------------
//    // -- Constructors/Assignment ---
//    // ------------------------------

//    // ------------------------
//    // --- Helper functions ---
//    // ------------------------
//private:

//private:
//    // --- Member data --
//    // --- Implementation remark: ---
//    // We do not use the simpler 'value_type  data_[CAPACITY];'
//    // data memeber since that would default initialize the entire array, which
//    // we don't want to spend time doing as the container conceptually is empty
//    // after construction, unless we are using one of the  initializing constructors.
//    // So: We simply want to reserve the memory at first.

//#if (CXX_STANDARD != 98)
////#   include "lock_guard__cpp11.hpp"
//#endif
//};


} // END namespace estl



#endif // ESTL_MUTEX_HPP
