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


template <typename Key, typename T, size_t CAPACITY, class Compare = std::less<Key>, size_t LINEAR_SEARCH_LIMIT = 10 >
struct sort_map_s
{
private:
    using node_t = std::pair<Key, T>;
    using vector_t = estl::vector_s<node_t, CAPACITY>;
public:
    using value_type                = std::pair<const Key, T>;
    using size_type                 = size_t;
    using difference_type           = ptrdiff_t;
    using reference                 = value_type&;
    using const_reference           = const value_type&;
    using pointer                   = value_type*;
    using const_pointer             = const value_type*;

    using iterator                  = typename vector_t::iterator;
    using const_iterator            = typename vector_t::const_iterator;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = const std::reverse_iterator<const_iterator>;


    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
public:
    sort_map_s() = default;

//    sort_map_s() {
//        std::cout << "sort_map_s default constructor this: " << this <<  std::endl;
//    };

    // -----------------
    // --- Iterators ---
    // -----------------
    iterator                begin()     noexcept { return data_vec_.begin(); }
    const_iterator          begin()     const noexcept { return data_vec_.begin(); }
    const_iterator          cbegin()    const noexcept { return data_vec_.cbegin(); }

    iterator                end()       noexcept { return data_vec_.end(); }
    const_iterator          end()       const noexcept { return data_vec_.end(); }
    const_iterator          cend()      const noexcept { return data_vec_.cend(); }

    reverse_iterator        rbegin()    noexcept { return data_vec_.rbegin(); }
    const_reverse_iterator  rbegin()    const noexcept { return data_vec_.rbegin(); }
    const_reverse_iterator  crbegin()   const noexcept { return data_vec_.crbegin(); }

    reverse_iterator        rend()      noexcept { return data_vec_.rend(); }
    const_reverse_iterator  rend()      const noexcept { return data_vec_.rend(); }
    const_reverse_iterator  crend()     const noexcept { return data_vec_.crend(); }

    // ----------------
    // --- Capacity ---
    // ----------------
    bool          empty       () const noexcept {   return size() == 0; }
    size_type     size        () const noexcept {   return data_vec_.size(); }
    size_type     max_size    () const noexcept {   return data_vec_.max_size(); }
    size_type     capacity    () const noexcept {   return data_vec_.capacity(); }

    // -----------------
    // --- Modifiers ---
    // -----------------
    void clear() noexcept
    {
        data_vec_.clear();
    }

    // --------------
    // --- Access ---
    // --------------
    T& operator[]( const Key& key )
    {
        if (key_is_last_element(key)) {
            return data_vec_.back().second;
        }
        auto it = do_find(key);
        if (it == end()) {
            it = do_insert_raw({key,T{}});
        }
        return it->second;
    }

    iterator insert(const value_type& value)
    {
        return do_insert_raw(value);

    }


private:
    // ---Helper functions --
//   std::pair<iterator,bool> do_insert(const value_type& )
    iterator do_insert_raw(const value_type& value)
    {
        is_sorted_ = false;
        data_vec_.push_back(value);
        return data_vec_.end() -1;
    }


    iterator do_find(const Key& key)
    {
        if (size() < LINEAR_SEARCH_LIMIT) {
            return do_find_linear(key);
        }
        if (!is_sorted_) {
            sort();
        }
//        return do_find_bisect(key);
        return end();
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

        const auto it_end = end();
        const auto it = std::lower_bound(data_vec_.begin(), it_end, key, compare_);
        return it != it_end && !compare_(key, *it) ? it : it_end;
    }

    iterator do_find_linear(const Key& key)
    {
        const auto it_end = end();
        for (auto it = begin(); it != it_end; ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return it_end;
    }

    void insertion_sort()
    {
        const auto end = data_vec_.end();
        const auto begin = data_vec_.begin();
        for (auto it = data_vec_.begin(); it != end; ++it)
        {
            auto const insertion_point = std::upper_bound(begin, it, *it , compare_);
            // Shifting the unsorted part
            std::rotate(insertion_point, it, it+1);
        }
    }

    void sort()
    {
//        std::sort(data_vec_.begin(), data_vec_.end(), std::less<std::string>());
        number_elements_added_since_last_sort_ = 0;
        is_sorted_ = true; // TODO: Do we need this when we have the above counter ?
    }

    /** Simple helper to test whether the key refers to the last inserted
        element. */
    bool key_is_last_element(const Key& key)
    {
        if (empty()) { return false; }
        return data_vec_.back().first == key;
    }


    // --- Member data --
    vector_t    data_vec_;
    Compare     compare_;
    size_type   number_elements_added_since_last_sort_ = 0U;
    bool        is_sorted_ = true;

};

} // END namespace cas

#endif // ESTL_SORT_MAP_S_HPP
