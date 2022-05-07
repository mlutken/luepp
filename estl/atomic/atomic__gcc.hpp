#include <atomic/atomic.hpp>

namespace estl {

template <class T>
inline constexpr atomic<T>::atomic() noexcept {}

template <class T>
constexpr atomic<T>::atomic(T desired) noexcept
    : value_(desired)
{
}

template <class T>
inline T atomic<T>::operator=( T desired ) noexcept
{
    __atomic_store (&value_, &desired, __ATOMIC_SEQ_CST);
    return *this;
}

template <class T>
inline T atomic<T>::operator=( T desired ) volatile noexcept
{
    __atomic_store (&value_, &desired, __ATOMIC_SEQ_CST);
    return *this;
}

template <class T>
inline atomic<T>::operator T() const noexcept
{
    T retval;
    __atomic_load (&value_, &retval, __ATOMIC_SEQ_CST);
    return retval;
}

template <class T>
inline atomic<T>::operator T() const volatile noexcept
{
    T retval;
    __atomic_load (&value_, &retval, __ATOMIC_SEQ_CST);
    return retval;
}

// Pre increment
template <class T>
inline T atomic<T>::operator++() noexcept
{
    return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator++() volatile noexcept
{
    return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

// Post increment
template <class T>
inline T atomic<T>::operator++(int) noexcept
{
    return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator++(int) volatile noexcept
{
    return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST));
}

// Pre decrement
template <class T>
inline T atomic<T>::operator--() noexcept
{
    return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator--() volatile noexcept
{
    return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST));
}

// Post decrement
template <class T>
inline T atomic<T>::operator--(int) noexcept
{
    return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator--(int) volatile noexcept
{
    return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator+=(T arg) noexcept
{
    return static_cast<T>(__atomic_fetch_add(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator+=(T arg) volatile noexcept
{
    return static_cast<T>(__atomic_fetch_add(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator-=(T arg) noexcept
{
    return static_cast<T>(__atomic_fetch_sub(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator-=(T arg) volatile noexcept
{
    return static_cast<T>(__atomic_fetch_sub(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator&=(T arg) noexcept
{
    return static_cast<T>(__atomic_fetch_and(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator&=(T arg) volatile noexcept
{
    return static_cast<T>(__atomic_fetch_and(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator|=(T arg) noexcept
{
    return static_cast<T>(__atomic_fetch_or(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator|=(T arg) volatile noexcept
{
    return static_cast<T>(__atomic_fetch_or(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator^=(T arg) noexcept
{
    return static_cast<T>(__atomic_fetch_xor(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline T atomic<T>::operator^=(T arg) volatile noexcept
{
    return static_cast<T>(__atomic_fetch_xor(&value_, arg, __ATOMIC_SEQ_CST));
}

template <class T>
inline bool atomic<T>::is_lock_free   () const noexcept
{
    return __atomic_always_lock_free (sizeof (T), 0);
}

template <class T>
inline bool atomic<T>::is_lock_free   () const volatile noexcept
{
    return __atomic_always_lock_free (sizeof (T), 0);
}

} // END namespace estl


