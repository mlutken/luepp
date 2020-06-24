#ifndef ESTL_SHARED_PTR_HPP
#define ESTL_SHARED_PTR_HPP
#include <nestle_default_config.h>

#include <stdexcept>
#include <memory>
#include <atomic/atomic_use.hpp>

// https://stackoverflow.com/questions/5671241/how-does-weak-ptr-work

// FIXMENM: Debug only!
#include <iostream>

namespace estl {
template <class T> class weak_ptr;

//typedef unsigned use_counter_type;
typedef estl_use::atomic<unsigned> use_counter_type;

template <class T>
class shared_ptr {
    friend class weak_ptr<T>;
    struct control_block {
        use_counter_type use_count_;
        use_counter_type weak_count_;
    };

public:
    shared_ptr()
        : ptr_(nullptr)
        , use_count_(nullptr)
    {
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
        : ptr_(w.ptr_),
          use_count_(w.use_count_)
    {
        incref();
    }

    void reset()
    {
        destroy();
    }

    void reset(T* p)
    {
        destroy();
        ptr_ = p;
        use_count_ = new use_counter_type(1);
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

    unsigned    use_count   () const
    {
        if (ptr_ == nullptr) return 0u;
        return *use_count_;
    }

//    unsigned    use_count   () const { return 0u; }
private:
//    /* special case, null pointer (nil-code) */
//    static use_counter_type* nil()
//    {
//        static use_counter_type nil_counter(0);
//        return &nil_counter;
//    }

    void decref()
    {
        if (!ptr_) {
            return;
        }
        std::cerr << "decref() before: " << *use_count_ << " ";
        if (--(*use_count_) == 0) {
            std::cerr << "  FIXMENM DELETING  ";
            std::cerr << "   decref() after: " << *use_count_ << "\n";
            destroy();
        }
        else {
            std::cerr << "   decref() after: " << *use_count_ << "\n";
        }
    }

    void incref()
    {
        std::cerr << "incref() before: " << *use_count_ << "\n";
        ++(*use_count_);
    }

    void destroy()
    {
        delete ptr_;
        delete use_count_;
        ptr_ = nullptr;
        use_count_ = nullptr;
    }

    friend class weak_ptr<T>;

    // --- PRIVATE: Member data ---
    T* ptr_;
    use_counter_type*   use_count_;
    control_block*      control_block_ptr_;
};

template <class T>
class weak_ptr {
    typedef typename shared_ptr<T>::control_block control_block;
public:
    friend class shared_ptr<T>;

    weak_ptr()
////        : ptr_(0), count_(shared_ptr<T>::nil())
        : ptr_(nullptr),
          use_count_(nullptr)
    {}

    explicit weak_ptr(const shared_ptr<T>& s)
        : ptr_(s.ptr_),
          use_count_(s.use_count_)
    {}

    weak_ptr<T>& operator=(const shared_ptr<T>& o)
    {
        if (ptr_ == o.ptr_) return *this;
        ptr_ = o.ptr_;
        use_count_ = o.use_count_;
        return *this;
    }

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

    unsigned    use_count   () const
    {
        if (ptr_ == nullptr) return 0u;
        return *use_count_;
    }

private:
    // --- PRIVATE: Member data ---
    T* ptr_;
    use_counter_type*   use_count_;
    control_block*      control_block_ptr_;
};

}

#endif // ESTL_SHARED_PTR_HPP
