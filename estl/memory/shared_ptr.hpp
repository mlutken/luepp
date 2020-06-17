#ifndef ESTL_SHARED_PTR_HPP
#define ESTL_SHARED_PTR_HPP
#include <nestle_default_config.h>

#include <stdexcept>
#include <memory>
#include <atomic/atomic_use.hpp>

namespace estl {
template <class T> class weak_ptr;

//typedef unsigned use_counter_type;
typedef estl_use::atomic<unsigned> use_counter_type;

template <class T>
class shared_ptr {
public:
    shared_ptr()
        : ptr_(0)
        , use_count_(nil())
    {
        incref();
    }

    ~shared_ptr()
    {
        decref();
    }

    shared_ptr(const shared_ptr<T>& o)
        : ptr_(o.ptr_)
        , use_count_(o.use_count_)
    {
        incref();
    }

    shared_ptr(T* p)
        : ptr_(p)
        , use_count_(new use_counter_type(1))
    {
    }

    explicit shared_ptr(const weak_ptr<T>& w)
        : ptr_(w.ptr_), use_count_(w.count_)
    {
        incref();
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& o)
    {
        if (ptr_ == o.ptr_) return *this;
        decref();
        ptr_ = o.ptr_;
        use_count_ = o.use_count_;
        incref();
        return *this;
    }

    T*          get()               { return ptr_;  }
    T*          operator->()        { return ptr_;  }
    T&          operator*()         { return *ptr_; }

    const T*    get() const         { return ptr_;  }
    const T*    operator->() const  { return ptr_;  }
    const T&    operator*() const   { return *ptr_; }

    explicit    operator bool   () const NESTLE_NOEXEPT         { return ptr_ != nullptr;   }
    bool        operator==      (const shared_ptr<T>& o) const  { return ptr_ == o.ptr_;    }
    bool        operator!=      (const shared_ptr<T>& o) const  { return ptr_ != o.ptr_;    }
    bool        operator<       (const shared_ptr<T>& o) const  { return ptr_ < o.ptr_;     }

    unsigned    use_count   () const { return *use_count_; }
private:
    /* special case, null pointer (nil-code) */
    static use_counter_type* nil()
    {
        static use_counter_type nil_counter(1);
        return &nil_counter;
    }

    void decref()
    {
        if (--(*use_count_) == 0) {
            delete ptr_;
            delete use_count_;
        }
    }

    void incref()
    {
        ++(*use_count_);
    }

    friend class weak_ptr<T>;

    // --- PRIVATE: Member data ---
    T* ptr_;
    use_counter_type* use_count_;
};

template <class T>
class weak_ptr {
public:
    friend class shared_ptr<T>;

    weak_ptr()
        : ptr_(0), count_(shared_ptr<T>::nil())
    {}

    explicit weak_ptr( const shared_ptr<T>& s)
        : ptr_(s.ptr_), count_(s.use_count_)
    {}

    shared_ptr<T> lock() const { return shared_ptr<T>(*this); }


    T*          get()               { return ptr_;  }
    T*          operator->()        { return ptr_;  }
    T&          operator*()         { return *ptr_; }

    const T*    get() const         { return ptr_;  }
    const T*    operator->  () const{ return ptr_;  }
    const T&    operator*   () const{ return *ptr_; }

    bool        operator==  (const shared_ptr<T>& o) const { return ptr_ == o.ptr_; }
    bool        operator!=  (const shared_ptr<T>& o) const { return ptr_ != o.ptr_; }
    bool        operator<   (const shared_ptr<T>& o) const { return ptr_ < o.ptr_; }

    unsigned    use_count() const { return *count_; }
private:
    // --- PRIVATE: Member data ---
    T* ptr_;
    use_counter_type* count_;
};

}

#endif // ESTL_SHARED_PTR_HPP
