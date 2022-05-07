#ifndef ESTL_ALLOCATOR_HPP
#define ESTL_ALLOCATOR_HPP

#include <stddef.h>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <memory>

// TODO: Add platform support. So projects can plugin their own.
// Perhaps with a policy class or with simple source file with platform "name"

namespace estl {
template<typename T>
class allocator
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer =const T*;
    using reference = T&;
    using const_reference =const T&;

    allocator() = default;
    allocator(const allocator&) = default; // construct by copying (do nothing)

    template<class OTHER>
    allocator(const allocator<OTHER>&) noexcept
    {
        // construct by copying (do nothing)
    }
    template<class OTHER>
    allocator<OTHER>& operator=(const allocator<OTHER>&)
    {	// assign from a related allocator (do nothing)
        return (*this);
    }


    template<class Other>
    struct rebind { typedef allocator<Other> other; };

    size_type max_size() const
    {
        return std::numeric_limits<unsigned>::max();
    }

    T* allocate(int n)
    {
        T* new_mem = static_cast<T*>(malloc(n * sizeof(T)));
        if (new_mem == nullptr) {
            // assert! throw std::bad_alloc();
        }
        return new_mem;
    }

    void construct(T* p, const T& v)
    {
        new(p) T{ v };
    }

    void construct(pointer p, T&& v)
    {
        new(p) T{ v };  // TODO: use std::forward or equivalent when possible
    }


    template<class OTHER>
    void construct(pointer p, OTHER&& v)
    {
        new(p) T{ v };  // TODO: use std::forward or equivalent when possible
    }

    void deallocate(T* p, int /*n*/)
    {
        free(p);
    }

    void destroy(T* p)
    {
        p->~T();
    }
};

template <class T, class U>
bool operator==(const estl::allocator<T>&, const estl::allocator<U>&) noexcept { return true; }
template <class T, class U>
bool operator!=(const estl::allocator<T>&, const estl::allocator<U>&) noexcept { return false; }

} // END namespace estl


#endif // ESTL_ALLOCATOR_HPP
