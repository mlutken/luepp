#ifndef ESTL_SORT_MAP_S_HPP
#define ESTL_SORT_MAP_S_HPP
#include <stddef.h>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <iostream>
namespace estl {

template <typename Key, typename T, size_t CAPACITY, class Compare = std::less<Key> >
struct sort_map_s
{
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
//    sort_map_s() {
//        std::cout << "sort_map_s default constructor this: " << this <<  std::endl;
//    };

    // --- Member data --
    value_type old_d_[CAPACITY];
    size_type size_ = 0u;
};

} // END namespace cas

#endif // ESTL_SORT_MAP_S_HPP
