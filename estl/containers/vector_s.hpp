#ifndef ESTL_VECTOR_S_HPP
#define ESTL_VECTOR_S_HPP
#include <nestle_default_config.h>
#include <cstdint>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <algorithm>


namespace estl {

template <typename T, size_t CAPACITY>
class vector_s
{
public:

    typedef T                                           value_type;
    typedef size_t                                      size_type;
    typedef ptrdiff_t                                   difference_type;
    typedef value_type&                                 reference;
    typedef const value_type&                           const_reference;
    typedef value_type*                                 pointer;
    typedef const value_type*                           const_pointer;
    typedef value_type*                                 iterator;
    typedef const value_type*                           const_iterator;
    typedef std::reverse_iterator<iterator>             reverse_iterator;
    typedef const std::reverse_iterator<const_iterator> const_reverse_iterator;

    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
    vector_s() = default;
    vector_s(size_type count, const T& value)
    {
        if (count > capacity()) {
            ESTL_THROW(std::range_error, "cas::vector_s constructing beyond capacity.");
        }
        for (size_t i = count; i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(value);
        }
        size_ = count;
    }

    explicit vector_s(size_type count)
    {
        if (count > capacity()) {
            ESTL_THROW(std::range_error, "cas::vector_s constructing beyond capacity.");
        }
        for (size_type i = count; i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) T(); // NOTE: We use () instead of {} to be compatible with C++98
        }
        size_ = count;
    }

    template<class InputIt>
    vector_s(const InputIt& first, const InputIt& last)
    {
        const size_type diff = check_range(first, last);
        size_type i = 0u;
        for (InputIt it = first; it != last; ++it, ++i) {
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(*it);
        }
        size_ = diff;
    }

    vector_s(const vector_s& other)
    {
        for (size_type i = other.size(); i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(other[i]);
        }
        size_ = other.size();
    }

    template<size_t CAPACITY_OTHER>
    vector_s(const vector_s<T, CAPACITY_OTHER>& other)
    {
        for (size_type i = other.size(); i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(other[i]);
        }
        size_ = other.size();
    }

    template<size_t CAPACITY_OTHER>
    vector_s(vector_s<T, CAPACITY_OTHER>&& other)
    {
        for (size_type i = other.size(); i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(NESTLE_MOVE(other[i]));
        }
        size_ = other.size();
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
    vector_s& operator=(const vector_s& other)
    {
        // NOTE: The reinterpret cast is needed as the pointer types can potentially
        //       be different if the capacities of the vectors are different.
        //       As we merely need to check the pointer adresses it's ok with
        //       with this "crude" cast here!
        if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) // prevent self-assignment
        {
            for (size_type i = other.size(); i > 0; ) {
                --i;
                const pointer insert_ptr = data_ptr_ + i;
                new (insert_ptr) value_type(other[i]);
            }
            size_ = other.size();
        }
        return *this;
    }

    template<size_t CAPACITY_OTHER>
    vector_s& operator=(const vector_s<T, CAPACITY_OTHER>& other)
    {
        // NOTE: The reinterpret cast is needed as the pointer types can potentially
        //       be different if the capacities of the vectors are different.
        //       As we merely need to check the pointer adresses it's ok with
        //       with this "crude" cast here!
        if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) // prevent self-assignment
        {
            for (size_type i = other.size(); i > 0; ) {
                --i;
                const pointer insert_ptr = data_ptr_ + i;
                new (insert_ptr) value_type(other[i]);
            }
            size_ = other.size();
        }
        return *this;
    }

    void assign(size_type count, const T& value)
    {
        if (count > capacity()) {
            ESTL_THROW (std::range_error, "cas::vector_s assigning beyond capacity.");
        }
        for (size_type i = count; i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(value);
        }
        size_ = count;
    }

    template< class InputIt >
    void assign(InputIt first, InputIt last)
    {
        const size_type diff = check_range(first, last);
        size_type i = 0u;
        for (InputIt it = first; it != last; ++it, ++i) {
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(*it);
        }
        size_ = diff;
    }


    ~vector_s() {
        destroy_elements();
    }

    // ----------------------
    // --- Element access ---
    // ----------------------
    reference at(size_type pos)
    {
        if (pos >= size()) {
            ESTL_THROW ( std::out_of_range, "cas::vector_s access (vector_s::at()) beyond size.");
        }
        return (*this)[pos];
    }

    const_reference at(size_type pos) const
    {
        if (pos >= size()) {
            ESTL_THROW (std::out_of_range, "cas::vector_s access (const vector_s::at()) beyond size.");
        }
        return (*this)[pos];
    }

    reference       operator[]( size_type pos ) { return *(data_ptr_ + pos); }
    const_reference operator[]( size_type pos ) const { return *(data_ptr_ + pos); }

    reference       front   ()                  { return (*this)[0]; }
    const_reference front   () const            { return (*this)[0]; }

    reference       back    ()                  { return (*this)[size_ -1]; }
    const_reference back    () const            { return (*this)[size_ -1]; }

    T*              data    () noexcept         { return &(*this)[0]; }
    const T*        data    () const noexcept   { return &(*this)[0]; }

    // -----------------
    // --- Iterators ---
    // -----------------
    iterator                begin()     NESTLE_NOEXEPT          { return &(*this)[0]; }
    const_iterator          begin()     const NESTLE_NOEXEPT    { return &(*this)[0]; }
    const_iterator          cbegin()    const NESTLE_NOEXEPT    { return &(*this)[0]; }

    iterator                end()       NESTLE_NOEXEPT          { return &(*this)[size_]; }
    const_iterator          end()       const NESTLE_NOEXEPT    { return &(*this)[size_]; }
    const_iterator          cend()      const NESTLE_NOEXEPT    { return &(*this)[size_]; }

    reverse_iterator        rbegin()    NESTLE_NOEXEPT          { return reverse_iterator(end()); }
    const_reverse_iterator  rbegin()    const NESTLE_NOEXEPT    { return const_reverse_iterator(cend()); }
    const_reverse_iterator  crbegin()   const NESTLE_NOEXEPT    { return const_reverse_iterator(cend()); }

    reverse_iterator        rend()      noexcept                { return reverse_iterator(begin()); }
    const_reverse_iterator  rend()      const NESTLE_NOEXEPT    { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator  crend()     const NESTLE_NOEXEPT    { return const_reverse_iterator(cbegin()); }

    // ----------------
    // --- Capacity ---
    // ----------------
    bool          empty       () const NESTLE_NOEXEPT {   return size_ == 0;    }
    size_type     size        () const NESTLE_NOEXEPT {   return size_;         }
    size_type     max_size    () const NESTLE_NOEXEPT {   return CAPACITY;      }
    size_type     capacity    () const NESTLE_NOEXEPT {   return CAPACITY;      }

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
        const size_type new_size = size() + 1u;
        if (new_size > capacity()) {
            ESTL_THROW (std::range_error, "cas::vector_s inserting beyond capacity.");
            return end();
        }

        iterator ipos = const_cast<iterator>(pos);
        shift_right(ipos, end(), 1u); // Allocates new elements
        const pointer insert_ptr = static_cast<pointer>(ipos);
        new (insert_ptr) value_type(value);
        // *ipos = value; // TODO: This seems ok too since the extra elements are already "allocated" by shift_right

        size_ = new_size;
        return ipos;
    }

    iterator insert(const_iterator pos, size_type count, const T& value)
    {
        const size_type new_size = size() + count;
        if (new_size > capacity()) {
            ESTL_THROW (std::range_error, "cas::vector_s inserting beyond capacity.");
            return end();
        }

        iterator ipos = const_cast<iterator>(pos);
        shift_right(ipos, end(), count);
        const iterator it_end = ipos + count;
        for (iterator it = ipos; it != it_end; ++it) {
            const pointer insert_ptr = static_cast<pointer>(it);
            new (insert_ptr) value_type(value);
            //*it = value; // TODO: This seems ok too since the extra elements are already "allocated" by shift_right
        }
        size_ = new_size;
        return ipos;
    }

    template< class InputIt >
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        const signed_size_t count_signed = last - first;
        if (count_signed < 0) {
            ESTL_THROW (std::out_of_range, "cas::vector_s range constructing/assigning from inverted range.");
            return end();
        }
        const size_type count = static_cast<size_type>(count_signed);
        const size_type new_size = size() + count;
        if (new_size > capacity()) {
            ESTL_THROW (std::range_error, "cas::vector_s inserting beyond capacity.");
            return end();
        }

        const iterator ipos_start = const_cast<iterator>(pos);
        iterator ipos = ipos_start;
        shift_right(ipos, end(), count);
        for (InputIt it = first; it != last; ++it, ++ipos) {
            const pointer insert_ptr = static_cast<pointer>(ipos);
            new (insert_ptr) value_type(*it);
            // *ipos = *it; // TODO: This seems ok too since the extra elements are already "allocated" by shift_right
        }
        size_ = new_size;
        return ipos_start;
    }

    iterator erase (const_iterator pos)
    {
        destroy(pos);
        iterator ipos = const_cast<iterator>(pos);
        shift_left(ipos+1, end(), 1u);
        --size_;
        return ipos;
    }

    iterator erase (const_iterator first, const_iterator last)
    {
		destroy(first, last);
        const size_type diff = check_range(first, last);
        const iterator ifirst = const_cast<iterator>(first);
        const iterator ilast = const_cast<iterator>(last);
        shift_left(ilast, end(), diff);
        size_ = size_ - diff;
        return ifirst;
    }

    void push_back (const T& value)
    {
        const size_type new_size = size() + 1;
        if (new_size > capacity()) {
            ESTL_THROW (std::range_error, "cas::vector_s push_back beyond capacity.");
        }
        const pointer insert_ptr = data_ptr_ + size();
        new (insert_ptr) value_type(value);
        size_ = new_size;
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
    void destroy(const_iterator begin, const_iterator end)
    {
        for (const_iterator it = begin; it != end; ++it) {
            destroy(it);
        }
    }

    void destroy(const_iterator pos)
    {
        iterator ipos = const_cast<iterator>(pos);
        pointer ptr = static_cast<pointer>(ipos);
        ptr->~T();
    }

    /** Destroy all elements. */
    void destroy_elements()
    {
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
        const signed_size_t diff_signed = last - first;
        if (diff_signed < 0) {
            ESTL_THROW (std::out_of_range, "cas::vector_s range constructing/assigning from inverted range.");
            return 0;
        }
        const size_type diff = static_cast<size_type>(diff_signed);
        if (diff > capacity()) {
            ESTL_THROW (std::range_error, "cas::vector_s constructing/assigning beyond capacity.");
            return 0;
        }
        return diff;
    }

    /** Shift range right by n elements. */
    template<class InputIt>
    void shift_right(InputIt first, InputIt last, size_type n)
    {
        const long diff_signed = last - first;
        if (diff_signed < 0) {
            return;
        }

        // Make room for elements
        for (size_type i = 0; i < n; ++i) {
            const pointer insert_ptr = last + i;
            new (insert_ptr) value_type(); // Default contruct the extra elements needed. Could perhaps construct when first needed instead!
        }

        for (InputIt it = last; it != first;) {
            --it;
            *(it+n) = NESTLE_MOVE(*it);
        }
    }

    /** Shift range left by n elements. */
    template<class InputIt>
    void shift_left(InputIt first,InputIt last, size_type n)
    {
        const long diff_signed = last - first;
        if (diff_signed < 0) {
            return;
        }
        for (InputIt it = first; it != last;) {
            *(it-n) = NESTLE_MOVE(*it);
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
    size_type       size_ = 0u;
    char            data_[CAPACITY*sizeof(value_type)]; // TODO: Use std::byte when we require C++17
    pointer         data_ptr_ = reinterpret_cast<pointer>(&data_[0]);

#if (CXX_STANDARD != 98)
#   include "vector_s__cpp11.hpp"
#endif
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

    for (typename vector_s<T, CAP_LHS>::size_type i = lhs.size(); i > 0; ) {
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

} // END namespace estl

#endif // ESTL_VECTOR_S_HPP
