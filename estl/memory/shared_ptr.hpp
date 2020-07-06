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

        void increase_use_count             () { ++use_count_; }
        void decrease_use_count             () { --use_count_; }
        void increase_control_block_count   () { ++control_block_count_;}
        void decrease_control_block_count   () { --control_block_count_;}


        ~control_block() {
            std::cerr << "*** FIXMENM ~control_block DESTRUCTOR " << this << "\n";
        }

        void dbg() {
            std::cerr << "*** FIXMENM use count: " << use_count_ << " control count: " << control_block_count_ <<  ",  this: " << this << "\n";
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
        , control_block_ptr_(new control_block(1, 1))
    {
    }

    explicit shared_ptr(const weak_ptr<T>& w)
        : ptr_(w.control_block_ptr_ && (w.control_block_ptr_->use_count_ > 0) ?  w.ptr_ : nullptr),
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
        decrease_counters();
        ptr_ = p;
        control_block_ptr_ = new control_block(1, 1);
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
        if (!ptr_ || !control_block_ptr_) {
            return;
        }

        std::cerr << "decref() "; control_block_ptr_->dbg();
        if (--(control_block_ptr_->use_count_) == 0) {
            destroy();
        }
        if (--(control_block_ptr_->control_block_count_) == 0) {
            destroy_control_block();
        }
    }

    void increase_counters()
    {
        if (!ptr_ || !control_block_ptr_) {
            return;
        }
        std::cerr << "incref() "; control_block_ptr_->dbg();
        control_block_ptr_->increase_counters();
    }

    void destroy()
    {
        std::cerr << "  FIXMENM destroy object\n";
        delete ptr_;
        ptr_ = nullptr;
    }

    void destroy_control_block()
    {
        std::cerr << "  FIXMENM delete control block\n";
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
        decrease_control_block_counter();
    }

    explicit weak_ptr(const shared_ptr<T>& s)
        : ptr_(s.ptr_),
          control_block_ptr_(s.control_block_ptr_)
    {
        increase_control_block_counter();
    }

    weak_ptr<T>& operator=(const shared_ptr<T>& o)
    {
        if (ptr_ == o.ptr_) return *this;
        ptr_ = o.ptr_;
        control_block_ptr_ = o.control_block_ptr_;
        if (control_block_ptr_) {
            control_block_ptr_->increase_control_block_count();
        }
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
    void increase_control_block_counter()
    {
        std::cerr << "control increase "; control_block_ptr_->dbg();
        if (control_block_ptr_) {
            control_block_ptr_->increase_control_block_count();
        }
    }

    void decrease_control_block_counter()
    {
        if (!control_block_ptr_) {
            return;
        }
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
