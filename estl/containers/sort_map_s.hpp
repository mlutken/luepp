#ifndef ESTL_SORT_MAP_S_HPP
#define ESTL_SORT_MAP_S_HPP

#include <algorithm>
#include "vector_s.hpp"

namespace estl {

/**
 *
 *
 * @references https://www.geeksforgeeks.org/insertion-sort-using-c-stl/
 *
 */


template <typename Key, typename T, size_t CAPACITY, class Compare = std::less<Key> >
struct sort_map_s
{
    using value_type                = std::pair<const Key, T>;
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

private:
    using node_t = std::pair<Key, T>;
    using data_t = estl::vector_s<node_t, CAPACITY>;

    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
public:
    sort_map_s() = default;

//    sort_map_s() {
//        std::cout << "sort_map_s default constructor this: " << this <<  std::endl;
//    };

//    T& operator[]( const Key& key )
//    {
//        do_insert_raw({key,})
//    }

    void insert(const value_type& value)
    {
        do_insert_raw(value);

    }


private:
    // ---Helper functions --
    //std::pair<iterator,bool> do_insert(const value_type& )
    void do_insert_raw(const value_type& value)
    {
        data_.push_back(value);

    }


    iterator do_find(const Key& key)
    {
        return do_find_bisect(key);
    }

//    template<class ForwardIt, class T, class Compare=std::less<>>
//    ForwardIt binary_find(ForwardIt first, ForwardIt last, const T& value, Compare comp={})
//    {
//        // Note: BOTH type T and the type after ForwardIt is dereferenced
//        // must be implicitly convertible to BOTH Type1 and Type2, used in Compare.
//        // This is stricter than lower_bound requirement (see above)

//        const auto it = std::lower_bound(data_.begin(), data_.end(), value, compare_);
//        return first != last && !comp(value, *first) ? first : last;
//    }

    iterator do_find_bisect(const Key& key)
    {
//        first = std::lower_bound(first, last, value, compare_);
//        return first != last && !comp(value, *first) ? first : last;

        const auto end = data_.end();
        const auto it = std::lower_bound(data_.begin(), end, key, compare_);
        return it != end && !compare_(key, *it) ? it : end;
    }

    void insertion_sort()
    {
        const auto end = data_.end();
        const auto begin = data_.begin();
        for (auto it = data_.begin(); it != end; ++it)
        {
            auto const insertion_point = std::upper_bound(begin, it, *it , compare_);
            // Shifting the unsorted part
            std::rotate(insertion_point, it, it+1);
        }
    }

    void sort()
    {
        std::sort(data_.begin(), data_.end(), compare_);
        number_elements_added_since_last_sort_ = 0;
        is_sorted_ = true; // TODO: Do we need this when we have the above counter ?
    }

    // --- Member data --
    data_t      data_;
    Compare     compare_;
    size_type   number_elements_added_since_last_sort_ = 0U;
    bool        is_sorted_ = true;

};

} // END namespace cas

#endif // ESTL_SORT_MAP_S_HPP
