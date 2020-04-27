#ifndef ESTL_CONST_SEQUENCE_HPP
#define ESTL_CONST_SEQUENCE_HPP
#include <nestle_default_config.h>

#include <stddef.h>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <memory>

// TODO:Temporary/development includes
#include <iostream>

namespace estl {
template <class T> class weak_ptr;

template <class T>
class shared_ptr {
    public:

        shared_ptr() : ptr(0), count(nil()) { incref(); }
        ~shared_ptr() { decref(); }

        shared_ptr(const shared_ptr<T>& o) : ptr(o.ptr), count(o.count) { incref(); }
        shared_ptr(T* p) : ptr(p), count(new unsigned(1)) {}
        explicit shared_ptr(const weak_ptr<T>& w) : ptr(w.ptr), count(w.count) { incref(); }

        shared_ptr<T>& operator=(const shared_ptr<T>& o) {
            if (ptr == o.ptr) return *this;
            decref();
            ptr = o.ptr;
            count = o.count;
            incref();
            return *this;
        }

        T* get() { return ptr; }
        T* operator->() { return ptr; }
        T& operator*()  { return *ptr; }

        const T* get() const { return ptr; }
        const T* operator->() const { return ptr; }
        const T& operator*() const { return *ptr; }

        explicit    operator bool() const NESTLE_NOEXEPT { return ptr != nullptr; }
        bool        operator==(const shared_ptr<T>& o) const { return ptr == o.ptr; }
        bool        operator!=(const shared_ptr<T>& o) const { return ptr != o.ptr; }
        bool        operator<(const shared_ptr<T>& o) const { return ptr < o.ptr; }

        unsigned use_count() const { return *count; }
private:
    T* ptr;
    unsigned* count; //

    /* special case, null pointer (nil-code) */
    static unsigned* nil() { static unsigned nil_counter(1); return &nil_counter; }

    void decref() { if (--(*count) == 0) { delete ptr; delete count; }}
    void incref() { ++(*count); }

    friend class weak_ptr<T>;

};

template <class T>
class weak_ptr {
        T* ptr;
        unsigned* count;

        friend class shared_ptr<T>;

    public:

        weak_ptr() : ptr(0), count(shared_ptr<T>::nil()) {}
        explicit weak_ptr( const shared_ptr<T>& s) : ptr(s.ptr), count(s.count) {}

        shared_ptr<T> lock() const { return shared_ptr<T>(*this); }


        T* get() { return ptr; }
        T* operator->() { return ptr; }
        T& operator*()  { return *ptr; }

        const T* get() const { return ptr; }
        const T* operator->() const { return ptr; }
        const T& operator*() const { return *ptr; }

        bool operator==(const shared_ptr<T>& o) const { return ptr == o.ptr; }
        bool operator!=(const shared_ptr<T>& o) const { return ptr != o.ptr; }
        bool operator<(const shared_ptr<T>& o) const { return ptr < o.ptr; }

        unsigned use_count() const { return *count; }

};

}


#endif // ESTL_CONST_SEQUENCE_HPP
