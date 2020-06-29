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
        explicit control_block(unsigned initial_use_count, unsigned initial_control_block_count) :
            use_count_(initial_use_count),
            control_block_count_(initial_control_block_count)
        {
            std::cerr << "*** FIXMENM control_block CONSTRUCTOR " << this << "\n";
        }
        void reset (unsigned initial_use_count, unsigned initial_control_block_count) {
            use_count_ = initial_use_count;
            control_block_count_ = initial_control_block_count;
        }

        void increase_counters() {
            ++use_count_;
            ++control_block_count_;
        }


        ~control_block() {
            std::cerr << "*** FIXMENM ~control_block DESTRUCTOR " << this << "\n";
        }

        void dbg() {
            std::cerr << "*** FIXMENM use count: " << use_count_ << " control count: " << control_block_count_ <<  " " << this << "\n";
        }

        use_counter_type use_count_;
        use_counter_type control_block_count_;
    };

public:
    shared_ptr()
        : ptr_(nullptr),
          control_block_ptr_(nullptr)
    {
    }

    ~shared_ptr()
    {
        decrease_counters();
    }

    shared_ptr(const shared_ptr<T>& o)
        : ptr_(o.ptr_),
          control_block_ptr_(o.control_block_ptr_)
    {
        increase_counters();
    }

    shared_ptr(T* p)
        : ptr_(p)
        , control_block_ptr_(new control_block(1, 0))
    {
    }

    explicit shared_ptr(const weak_ptr<T>& w)
        : ptr_(w.ptr_),
          control_block_ptr_(w.control_block_ptr_)
    {
        increase_counters();
    }

    void reset()
    {
        destroy();
    }

    void reset(T* p)
    {
        destroy();
        ptr_ = p;
        if (control_block_ptr_) {
            control_block_ptr_->reset(1, 0);
        }
        else {
            control_block_ptr_ = new control_block(1, 0);
        }
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& o)
    {
        if (ptr_ == o.ptr_) return *this;
        decrease_counters();
        ptr_ = o.ptr_;
        control_block_ptr_ = o.control_block_ptr_;
        increase_counters();
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
        return control_block_ptr_->use_count_;
    }

private:
    void decrease_counters()
    {
        if (!ptr_) {
            return;
        }

        std::cerr << "decref() "; control_block_ptr_->dbg();
        if (--(control_block_ptr_->use_count_) == 0) {
            std::cerr << "  FIXMENM DELETING  ";
//            std::cerr << "   decref() after: " << *use_count_ << "\n";
            destroy();
        }
        else {
//            std::cerr << "   decref() after: " << *use_count_ << "\n";
        }
    }

    void increase_counters()
    {
        std::cerr << "incref() "; control_block_ptr_->dbg();
        control_block_ptr_->increase_counters();
    }

    void destroy()
    {
        delete ptr_;
        ptr_ = nullptr;
        check_and_destroy_control_block();
    }

    void check_and_destroy_control_block()
    {
        if (control_block_ptr_->control_block_count_ > 0) {
            return;
        }
        delete control_block_ptr_;
        control_block_ptr_ = nullptr;
    }

    friend class weak_ptr<T>;
    // --- PRIVATE: Member data ---
    T*              ptr_;
    control_block*  control_block_ptr_;
};

template <class T>
class weak_ptr {
    typedef typename shared_ptr<T>::control_block control_block;
public:
    friend class shared_ptr<T>;

    weak_ptr()
        : ptr_(nullptr),
          control_block_ptr_(nullptr)
    {}

    ~weak_ptr()
    {
        decrease_counter();
    }

    explicit weak_ptr(const shared_ptr<T>& s)
        : ptr_(s.ptr_),
          control_block_ptr_(s.control_block_ptr_)
    {
        increase_counter();
    }

    weak_ptr<T>& operator=(const shared_ptr<T>& o)
    {
        if (ptr_ == o.ptr_) return *this;
        ptr_ = o.ptr_;
        control_block_ptr_ = o.control_block_ptr_;
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
        return control_block_ptr_->use_count_;
    }

private:
    void increase_counter()
    {
        std::cerr << "control increase "; control_block_ptr_->dbg();
        ++(control_block_ptr_->control_block_count_);
    }

    void decrease_counter()
    {
        std::cerr << "control decrease "; control_block_ptr_->dbg();
        if (--(control_block_ptr_->control_block_count_) == 0) {
            delete control_block_ptr_;
            control_block_ptr_ = nullptr;
        }
    }

    // --- PRIVATE: Member data ---
    T*              ptr_;
    control_block*  control_block_ptr_;
};

}

#endif // ESTL_SHARED_PTR_HPP
