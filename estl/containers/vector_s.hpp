#ifndef ESTL_VECTOR_S_HPP
#define ESTL_VECTOR_S_HPP

#include <stddef.h>
#include <iterator>
#include <stdexcept>
#include <algorithm>

// TODO:Temporary/development includes
#include <iostream>

namespace estl {

template <typename T, size_t CAPACITY>
class vector_s
{
public:
    using value_type = T;
    //using allocator_type = void; // TODO: Can we do something clever with this?
    using size_type                 = size_t;
    using difference_type           = ptrdiff_t;
    using reference                 = value_type&;
    using const_reference           = const value_type&;
    using pointer                   = value_type*;
    using const_pointer             = const value_type*;

    using iterator                  = value_type*;
    using const_iterator            = const value_type*;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = const std::reverse_iterator<const_iterator>;

    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
    vector_s() = default;
    vector_s(size_type count, const T& value)
    {
        if (count > capacity()) {
            throw std::range_error{"cas::vector_s constructing beyond capacity."};
        }
        for (auto i = count; i > 0; ) {
            --i;
            (*this)[i] = value;
        }
        size_ = count;
    }

    explicit vector_s(size_type count)
    {
        if (count > capacity()) {
            throw std::range_error{"cas::vector_s constructing beyond capacity."};
        }
        for (auto i = count; i > 0; ) {
            --i;
            (*this)[i] = T{};
        }
        size_ = count;
    }

    template<class InputIt>
    vector_s(InputIt first, InputIt last)
    {
        const auto diff = check_range(first, last);
        size_type i = 0u;
        for (auto it = first; it != last; ++it, ++i) {
            (*this)[i] = *it;
        }
        size_ = diff;
    }

    template<size_t CAPACITY_OTHER>
    vector_s(const vector_s<T, CAPACITY_OTHER>& other)
    {
        for (auto i = other.size(); i > 0; ) {
            --i;
            (*this)[i] = other[i];
        }
        size_ = other.size();
    }

//    vector_s( vector_s&& other ) noexcept; // Not relevant here, I think!

    // NOTE: Seems the use of initializer_list forces the copy constructor to be called
    //      https://stackoverflow.com/questions/13148772/in-place-vector-construction-from-initialization-list-for-class-with-constructo
    //      This is quite annoying a seems like a wase op CPU cycles for no good reason
    //      We really should allow the C++ standrd to construct the element in place like
    //      when doing emplace_back.
    vector_s(const std::initializer_list<T>& init)
    {
//        std::cout << "vector_s(const std::initializer_list<T>& init) constructor this: " << this <<  std::endl;
        auto it = init.begin();
        const auto end = init.end();
        const auto diff = check_range(it, end);
        size_type i = 0u;
        for (; it != end; ++it, ++i) {
            (*this)[i] = std::move(*it);
        }
        size_ = diff;
    }

    /** Assignment from other vector.

    @note: We do not use the common copy construct and swap idiom
    as shown here:
    @code
    MyClass& operator=(MyClass other)
    {
        swap(other);
        return *this;
    }
    @endcode
    OR here:
    MyClass& operator=(const MyClass& other)
    {
        MyClass tmp(other);
        swap(tmp);
        return *this;
    }
    This is because the swap function will not be very efficient given the
    static nature of this vector: swap needs to copy all elements and not
    just swap pointers as std::vector::swap() can do.
    */
    template<size_t CAPACITY_OTHER>
    vector_s& operator=(const vector_s<T, CAPACITY_OTHER>& other)
    {
        // NOTE: The reinterpret cast is needed as the pointer types can potentially
        //       be different if the capacities of the vectors are different.
        //       As we merely need to check the pointer adresses it's ok with
        //       with this "crude" cast here!
        if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) // prevent self-assignment
        {
            for (auto i = other.size(); i > 0; ) {
                --i;
                (*this)[i] = other[i];
            }
            size_ = other.size();
        }

        return *this;
    }

    vector_s& operator=(std::initializer_list<T> ilist)
    {
        auto it = ilist.begin();
        const auto end = ilist.end();
        const auto diff = check_range(it, end);
        size_type i = 0u;
        for (; it != end; ++it, ++i) {
            (*this)[i] = *it;
        }
        size_ = diff;
        return *this;
    }

    void assign(size_type count, const T& value)
    {
        if (count > capacity()) {
            throw std::range_error{"cas::vector_s assigning beyond capacity."};
        }
        for (auto i = count; i > 0; ) {
            --i;
            (*this)[i] = value;
        }
        size_ = count;
    }

    template< class InputIt >
    void assign(InputIt first, InputIt last)
    {
        const auto diff = check_range(first, last);
        size_type i = 0u;
        for (auto it = first; it != last; ++it, ++i) {
            (*this)[i] = *it;
        }
        size_ = diff;
    }

    void assign(std::initializer_list<T> ilist)
    {
        auto it = ilist.begin();
        const auto end = ilist.end();
        const auto diff = check_range(it, end);
        size_type i = 0u;
        for (; it != end; ++it, ++i) {
            (*this)[i] = *it;
        }
        size_ = diff;
    }

    ~vector_s() {
        destroy_elements();
    }

    // ----------------------
    // --- Element access ---
    // ----------------------
    reference at( size_type pos )
    {
        if (pos >= size()) {
            throw std::out_of_range{"cas::vector_s access (vector_s::at()) beyond size."};
        }
        return (*this)[pos];
    }

    const_reference at( size_type pos ) const
    {
        if (pos >= size()) {
            throw std::out_of_range{"cas::vector_s access (const vector_s::at()) beyond size."};
        }
        return (*this)[pos];
    }

    reference       operator[]( size_type pos ) { return *(data_ptr_ + pos); }
    const_reference operator[]( size_type pos ) const { return *(data_ptr_ + pos); }

    reference       front() { return (*this)[0]; }
    const_reference front() const { return (*this)[0]; }

    reference       back() { return (*this)[size_ -1]; }
    const_reference back() const { return (*this)[size_ -1]; }

    T*              data() noexcept { return &(*this)[0]; }
    const T*        data() const noexcept { return &(*this)[0]; }

    // -----------------
    // --- Iterators ---
    // -----------------
    iterator                begin()     noexcept { return &(*this)[0]; }
    const_iterator          begin()     const noexcept { return &(*this)[0]; }
    const_iterator          cbegin()    const noexcept { return &(*this)[0]; }

    iterator                end()       noexcept { return &(*this)[size_]; }
    const_iterator          end()       const noexcept { return &(*this)[size_]; }
    const_iterator          cend()      const noexcept { return &(*this)[size_]; }

    reverse_iterator        rbegin()    noexcept { return reverse_iterator(end()); }
    const_reverse_iterator  rbegin()    const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator  crbegin()   const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator        rend()      noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator  rend()      const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator  crend()     const noexcept { return const_reverse_iterator(cbegin()); }

    // ----------------
    // --- Capacity ---
    // ----------------
    bool          empty       () const noexcept {   return size_ == 0; }
    size_type     size        () const noexcept {   return size_; }
    size_type     max_size    () const noexcept {   return CAPACITY; }
    size_type     capacity    () const noexcept {   return CAPACITY; }

    // -----------------
    // --- Modifiers ---
    // -----------------
    void clear() noexcept
    {
        destroy_elements();
        size_ = 0;
    }

    iterator insert(const_iterator pos, const T& value)
    {
        const auto new_size = size() + 1u;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s inserting beyond capacity."};
        }

        iterator ipos = const_cast<iterator>(pos);
        shift_right(ipos, end(), 1u);
        *ipos = value;
        size_ = new_size;
        return ipos;
    }

    iterator insert(const_iterator pos, const T&& value)
    {
        const auto new_size = size() + 1u;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s inserting beyond capacity."};
        }

        iterator ipos = const_cast<iterator>(pos);
        shift_right(ipos, end(), 1u);
        *ipos = std::move(value);
        size_ = new_size;
        return ipos;
    }


    iterator insert(const_iterator pos, size_type count, const T& value)
    {
        const auto new_size = size() + count;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s inserting beyond capacity."};
        }

        iterator ipos = const_cast<iterator>(pos);
        shift_right(ipos, end(), count);
        const auto it_end = ipos + count;
        for (auto it = ipos; it != it_end; ++it) {
            *it = value;
        }
        size_ = new_size;
        return ipos;
    }

    template< class InputIt >
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        const auto count_signed = last - first;
        if (count_signed < 0) {
            throw std::out_of_range{"cas::vector_s range constructing/assigning from inverted range."};
        }
        const auto count = static_cast<size_type>(count_signed);
        const auto new_size = size() + count;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s inserting beyond capacity."};
        }

        const iterator ipos_start = const_cast<iterator>(pos);
        iterator ipos = ipos_start;
        shift_right(ipos, end(), count);
        for (auto it = first; it != last; ++it, ++ipos) {
            *ipos = *it;
        }
        size_ = new_size;
        return ipos_start;
    }

    iterator insert (const_iterator pos, std::initializer_list<T> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template< class... Args >
    iterator emplace( const_iterator pos, Args&&... args )
    {
        const auto new_size = size() + 1u;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s emplace beyond capacity."};
        }

        iterator ipos = const_cast<iterator>(pos);
        shift_right(ipos, end(), 1u);
        new (static_cast<pointer>(ipos)) T(std::forward<Args>(args)...);
        size_ = new_size;
        return ipos;
    }

    iterator erase (const_iterator pos)
    {
        iterator ipos = const_cast<iterator>(pos);
        shift_left(ipos+1, end(), 1u);
        --size_;
        return ipos;
    }

    iterator erase (const_iterator first, const_iterator last)
    {
        const auto diff = check_range(first, last);
        const iterator ifirst = const_cast<iterator>(first);
        const iterator ilast = const_cast<iterator>(last);
        shift_left(ilast, end(), diff);
        const auto new_size = size_ - diff;
        size_ = static_cast<size_type>(new_size);
        return ifirst;
    }

    void push_back (const T& value)
    {
        const auto new_size = size() + 1;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s push_back beyond capacity."};
        }
        (*this)[size()] = value;
        size_ = new_size;
    }

    void push_back (T&& value)
    {
        const auto new_size = size() + 1;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s push_back beyond capacity."};
        }
        (*this)[size()] = std::move(value);
        size_ = new_size;
    }


    template<class... Args >
    reference emplace_back(Args&&... args)
    {
        const auto new_size = size() + 1;
        if (new_size > capacity()) {
            throw std::range_error{"cas::vector_s emplace_back beyond capacity."};
        }

        const pointer insert_ptr = data_ptr_ + size();
        new (insert_ptr) value_type(std::forward<Args>(args)...);
        size_ = new_size;
        return *insert_ptr;
    }

    void pop_back()
    {
        --size_;
    }

    template<size_t CAPACITY_OTHER>
    void swap(vector_s<T, CAPACITY_OTHER>& other) noexcept
    {
        if ( other.size() > capacity() ||
             other.capacity() < size() ) {
            return;
        }
        vector_s temp(*this);
        *this = other;
        other = temp;
    }

    // ------------------------
    // --- Helper functions ---
    // ------------------------
private:
    /** Destroy all elements. */
    void destroy_elements() {
        for (size_type i = size(); i > 0; ) {
            --i;
            pointer p = &(*this)[i];
            p->~T();
        }
    }

    /** Check range */
    template<class InputIt>
    size_type check_range(InputIt first, InputIt last)
    {
        const auto diff_signed = last - first;
        if (diff_signed < 0) {
            throw std::out_of_range{"cas::vector_s range constructing/assigning from inverted range."};
        }
        const auto diff = static_cast<size_type>(diff_signed);
        if (diff > capacity()) {
            throw std::range_error{"cas::vector_s constructing/assigning beyond capacity."};
        }
        return diff;
    }

    /** Shift range right by n elements. */
    template<class InputIt>
    void shift_right(InputIt first, InputIt last, size_type n)
    {
        const auto diff_signed = last - first;
        if (diff_signed < 0) {
            return;
        }
        for (auto it = last; it != first;) {
            --it;
            *(it+n) = std::move(*it);
        }
    }

    /** Shift range left by n elements. */
    template<class InputIt>
    void shift_left(InputIt first, InputIt last, size_type n)
    {
        const auto diff_signed = last - first;
        if (diff_signed < 0) {
            return;
        }
        for (auto it = first; it != last;) {
            *(it-n) = std::move(*it);
            ++it;
        }
    }

private:
    // --- Member data --
    // --- Implementation remark: ---
    // We do not use the simpler 'value_type  data_[CAPACITY];'
    // data memeber since that would default initialize the entire array, which
    // we don't want to spend time doing as the container conceptually is empty
    // after construction, unless we are using one of the  initializing constructors.
    // So: We simply want to reserve the memory at first.
    size_type   size_ = 0u;
    uint8_t     data_[CAPACITY*sizeof(value_type)];
    pointer     data_ptr_ = reinterpret_cast<pointer>(&data_[0]);
};

// ------------------------
// -- Compare operators ---
// ------------------------
template <typename T, size_t CAP_LHS, size_t CAP_RHS>
bool operator==( const vector_s<T, CAP_LHS>& lhs, const vector_s<T, CAP_RHS>& rhs )
{
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (auto i = lhs.size(); i > 0; ) {
        --i;
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template <typename T, size_t CAP_LHS, size_t CAP_RHS>
bool operator!=( const vector_s<T, CAP_LHS>& lhs, const vector_s<T, CAP_RHS>& rhs )
{
    return !(lhs == rhs);
}

} // END namespace cas

#endif // ESTL_VECTOR_S_HPP
