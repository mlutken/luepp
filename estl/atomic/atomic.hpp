/**@file
 * C++11 atomic operation for use when yout don't have C++11
 * standard library available.
 */
#ifndef ESTL_ATOMIC_H
#define ESTL_ATOMIC_H
#include <stdbool.h>

namespace estl {

template <class T>
class atomic
{
public:
    constexpr atomic() noexcept = default;
    constexpr atomic(T desired)
    {
    }

    atomic(const atomic&) = delete;

    T operator=( T desired ) noexcept;
    T operator=( T desired ) volatile noexcept;

    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;

//    inline long InterlockedIncrement(long* p)
//    {
//        return __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
//    }

    // Pre increment
    T operator++() noexcept             { return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator++() volatile noexcept    { return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    // Post increment
    T operator++(int) noexcept          { return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator++(int) volatile noexcept { return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST)); }

    // Pre decrement
    T operator--() noexcept             { return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator--() volatile noexcept    { return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    // Post decrement
    T operator--(int) noexcept          { return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator--(int) volatile noexcept { return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST)); }

    T operator+=( T arg ) noexcept;
    T operator+=( T arg ) volatile noexcept;
        

private:
    T value_;
};



#if defined(__clang__)

#elif defined(__GNUG__)


#elif defined(_MSC_VER)

#endif




} // END namespace estl

//#if (C_STANDARD == 11)
//#include <stdatomic.h>
//#define cesl_sizeof_atomic_flag sizeof(atomic_flag)
//inline bool cesl_atomic_flag_test_and_set(volatile atomic_flag* obj)
//{
//    return atomic_flag_test_and_set(atomic_flag);
//}

//inline bool cesl_atomic_flag_test_and_set_explicit(volatile atomic_flag* obj, memory_order order)
//{
//    return atomic_flag_test_and_set_explicit(obj, order);
//}

//#elif (C_STANDARD == 99)
//// If complier gcc: TODO: Other compilers
//#define cesl_sizeof_atomic_flag 1

//inline bool cesl_atomic_flag_test_and_set(volatile void* obj)
//{
//    return __atomic_test_and_set (obj, __ATOMIC_SEQ_CST);
//}

//inline bool cesl_atomic_flag_test_and_set_explicit(volatile void* obj, int memory_order)
//{
//    return __atomic_test_and_set (obj, memory_order);
//}


//#define cesl_atomic_store(object, desired) __sync_lock_test_and_set(object, desired)
//#define cesl_atomic_load(object) __sync_fetch_and_add(object, 0 )

//#endif

#endif // ESTL_ATOMIC_H
