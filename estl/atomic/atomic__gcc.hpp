#include <atomic/atomic.hpp>

namespace estl {

template <class T>
inline constexpr atomic<T>::atomic() NESTLE_NOEXEPT {}

template <class T>
constexpr atomic<T>::atomic(T desired) NESTLE_NOEXEPT
    : value_(desired)
{
}

template <class T>
inline T atomic<T>::operator=( T desired ) NESTLE_NOEXEPT
{
    __atomic_store (&value_, &desired, __ATOMIC_SEQ_CST);
    return *this;
}

template <class T>
inline T atomic<T>::operator=( T desired ) volatile NESTLE_NOEXEPT
{
    __atomic_store (&value_, &desired, __ATOMIC_SEQ_CST);
    return *this;
}

template <class T>
inline atomic<T>::operator T() const NESTLE_NOEXEPT
{
    T retval;
    __atomic_load (&value_, &retval, __ATOMIC_SEQ_CST);
    return retval;
}

template <class T>
inline atomic<T>::operator T() const volatile NESTLE_NOEXEPT
{
    T retval;
    __atomic_load (&value_, &retval, __ATOMIC_SEQ_CST);
    return retval;
}

// Pre increment
template <class T>
inline T atomic<T>::operator++() NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator++() volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

// Post increment
template <class T>
inline T atomic<T>::operator++(int) NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator++(int) volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST));
}

// Pre decrement
template <class T>
inline T atomic<T>::operator--() NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator--() volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

// Post decrement
template <class T>
inline T atomic<T>::operator--(int) NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator--(int) volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator+=(T arg) NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_add(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator+=(T arg) volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_add(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator-=(T arg) NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_sub(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator-=(T arg) volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_sub(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator&=(T arg) NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_and(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator&=(T arg) volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_and(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator|=(T arg) NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_or(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator|=(T arg) volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_or(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator^=(T arg) NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_xor(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator^=(T arg) volatile NESTLE_NOEXEPT
{
    return static_cast<T>(__atomic_fetch_xor(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline bool atomic<T>::is_lock_free   () const NESTLE_NOEXEPT
{
    return __atomic_always_lock_free (sizeof (T), 0);
}

template <class T>
inline bool atomic<T>::is_lock_free   () const volatile NESTLE_NOEXEPT
{
    return __atomic_always_lock_free (sizeof (T), 0);
}

} // END namespace estl


