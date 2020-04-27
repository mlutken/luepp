public:
    constexpr atomic() NESTLE_NOEXEPT {}

    constexpr atomic(T desired) NESTLE_NOEXEPT
        : value_(desired)
    {
    }

#if (CXX_STANDARD != 98)
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
#endif

    T operator=( T desired ) NESTLE_NOEXEPT           { __atomic_store (&value_, &desired, __ATOMIC_SEQ_CST); return *this; }
    T operator=( T desired ) volatile NESTLE_NOEXEPT  { __atomic_store (&value_, &desired, __ATOMIC_SEQ_CST); return *this; }


//    inline long InterlockedIncrement(long* p)
//    {
//        return __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
//    }
    operator T() const NESTLE_NOEXEPT           { T retval; __atomic_load (&value_, &retval, __ATOMIC_SEQ_CST); return retval; }
    operator T() const volatile NESTLE_NOEXEPT  { T retval; __atomic_load (&value_, &retval, __ATOMIC_SEQ_CST); return retval; }

    // Pre increment
    T operator++() NESTLE_NOEXEPT               { return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator++() volatile NESTLE_NOEXEPT      { return static_cast<T>(__atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    // Post increment
    T operator++(int) NESTLE_NOEXEPT            { return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator++(int) volatile NESTLE_NOEXEPT   { return static_cast<T>(__atomic_fetch_add(&value_, 1, __ATOMIC_SEQ_CST)); }

    // Pre decrement
    T operator--() NESTLE_NOEXEPT               { return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator--() volatile NESTLE_NOEXEPT      { return static_cast<T>(__atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST)); }
    // Post decrement
    T operator--(int) NESTLE_NOEXEPT            { return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST)); }
    T operator--(int) volatile NESTLE_NOEXEPT   { return static_cast<T>(__atomic_fetch_sub(&value_, 1, __ATOMIC_SEQ_CST)); }

    T operator+=(T arg) NESTLE_NOEXEPT          {return static_cast<T>(__atomic_fetch_add(&value_, arg, __ATOMIC_SEQ_CST)); }
    T operator+=(T arg) volatile NESTLE_NOEXEPT {return static_cast<T>(__atomic_fetch_add(&value_, arg, __ATOMIC_SEQ_CST)); }

    T operator-=(T arg) NESTLE_NOEXEPT          {return static_cast<T>(__atomic_fetch_sub(&value_, arg, __ATOMIC_SEQ_CST)); }
    T operator-=(T arg) volatile NESTLE_NOEXEPT {return static_cast<T>(__atomic_fetch_sub(&value_, arg, __ATOMIC_SEQ_CST)); }

    T operator&=(T arg) NESTLE_NOEXEPT          {return static_cast<T>(__atomic_fetch_and(&value_, arg, __ATOMIC_SEQ_CST)); }
    T operator&=(T arg) volatile NESTLE_NOEXEPT {return static_cast<T>(__atomic_fetch_and(&value_, arg, __ATOMIC_SEQ_CST)); }

    T operator|=(T arg) NESTLE_NOEXEPT          {return static_cast<T>(__atomic_fetch_or(&value_, arg, __ATOMIC_SEQ_CST)); }
    T operator|=(T arg) volatile NESTLE_NOEXEPT {return static_cast<T>(__atomic_fetch_or(&value_, arg, __ATOMIC_SEQ_CST)); }

    T operator^=(T arg) NESTLE_NOEXEPT          {return static_cast<T>(__atomic_fetch_xor(&value_, arg, __ATOMIC_SEQ_CST)); }
    T operator^=(T arg) volatile NESTLE_NOEXEPT {return static_cast<T>(__atomic_fetch_xor(&value_, arg, __ATOMIC_SEQ_CST)); }

    bool is_lock_free   () const NESTLE_NOEXEPT            { return __atomic_always_lock_free (sizeof (T), 0); }
    bool is_lock_free   () const volatile NESTLE_NOEXEPT   { return __atomic_always_lock_free (sizeof (T), 0); }

private:
    volatile mutable T value_;


