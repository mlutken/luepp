#ifndef ESTL_SORT_MAP_S_HPP
#define ESTL_SORT_MAP_S_HPP

#include <nestle_default_config.h>
#include <algorithm>
#include <functional>
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
    typedef std::pair<Key, T>                           node_t;
    typedef estl::vector_s<node_t, CAPACITY>            vector_t;
public:
    typedef Key                                         key_type;
    typedef std::pair<const Key, T>                     value_type;
    typedef size_t                                      size_type;
    typedef ptrdiff_t                                   difference_type;
    typedef value_type&                                 reference;
    typedef const value_type&                           const_reference;
    typedef value_type*                                 pointer;
    typedef const value_type*                           const_pointer;
    typedef typename vector_t::iterator                 iterator;
    typedef typename vector_t::const_iterator           const_iterator;
    typedef std::reverse_iterator<iterator>             reverse_iterator;
    typedef const std::reverse_iterator<const_iterator> const_reverse_iterator;

    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
public:
    sort_map_s()
        : comperator_(Compare())
    {}

    explicit sort_map_s(const Compare& comperator)
        : comperator_(comperator)
    {}


    // ----------------------
    // --- Element access ---
    // ----------------------
    T& operator[]( const Key& key )
    {
        if (key_is_last_element(key)) {
            return data_vec_.back().second;
        }
        iterator it = do_find(key);
        if (it == end()) {
            it = do_insert_raw({key,T()});
        }
        return it->second;
    }


    // -----------------
    // --- Iterators ---
    // -----------------
    iterator                begin   ()  NESTLE_NOEXEPT          { return data_vec_.begin(); }
    const_iterator          begin   ()  const NESTLE_NOEXEPT    { return data_vec_.begin(); }
    const_iterator          cbegin  ()  const NESTLE_NOEXEPT    { return data_vec_.cbegin(); }

    iterator                end     ()  NESTLE_NOEXEPT          { return data_vec_.end(); }
    const_iterator          end     ()  const NESTLE_NOEXEPT    { return data_vec_.end(); }
    const_iterator          cend    ()  const NESTLE_NOEXEPT    { return data_vec_.cend(); }

    reverse_iterator        rbegin  ()  NESTLE_NOEXEPT          { return data_vec_.rbegin(); }
    const_reverse_iterator  rbegin  ()  const NESTLE_NOEXEPT    { return data_vec_.rbegin(); }
    const_reverse_iterator  crbegin ()  const NESTLE_NOEXEPT    { return data_vec_.crbegin(); }

    reverse_iterator        rend    ()  NESTLE_NOEXEPT          { return data_vec_.rend(); }
    const_reverse_iterator  rend    ()  const NESTLE_NOEXEPT    { return data_vec_.rend(); }
    const_reverse_iterator  crend   ()  const NESTLE_NOEXEPT    { return data_vec_.crend(); }

    // ----------------
    // --- Capacity ---
    // ----------------
    bool                    empty       () const NESTLE_NOEXEPT {   return size() == 0; }
    size_type               size        () const NESTLE_NOEXEPT {   return data_vec_.size(); }
    size_type               max_size    () const NESTLE_NOEXEPT {   return data_vec_.max_size(); }
    size_type               capacity    () const NESTLE_NOEXEPT {   return data_vec_.capacity(); }

    // -----------------
    // --- Modifiers ---
    // -----------------
    void clear() noexcept
    {
        data_vec_.clear();
    }

    iterator insert(const value_type& value)
    {
        return do_insert_raw(value);
    }

    iterator erase(iterator pos)
    {
        destroy(pos);
        is_sorted_ = false;
        return pos;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        for (const_iterator it = first; it != last; ++it) {
            destroy(it);
        }
        is_sorted_ = false;
        return first;
    }

    size_type erase(const key_type& key)
    {
        const const_iterator pos = find(key);
        if (pos != end()) {
            erase(pos);
            return 1;
        }
        return 0;
    }

    // --------------
    // --- Lookup ---
    // --------------

    iterator find(const Key& key) {
        return do_find(key);
    }

    const_iterator find(const Key& key) const
    {
        return static_cast<const_iterator>(do_find(key));
    }

    bool contains (const Key& key) const
    {
        return find(key) != end();
    }


private:
    struct comperator
    {
        comperator(Compare compare) : compare_(compare){}
        bool operator()(const value_type& lhs, const value_type& rhs) {
            return compare_(lhs.first, rhs.first);
        }
        Compare compare_;
    };


    // ---Helper functions --
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
        return do_find_bisect(key);
    }

    iterator do_find_bisect (const Key& key)
    {
        //std::cerr << "do_find_bisect(" << key << ")\n";
        const value_type elem (key,T());

        const iterator it_end = end();
        const iterator it = std::lower_bound(data_vec_.begin(), it_end, elem, comperator_);
        return !(it == it_end) && !comperator_(elem, *it) ? it : it_end;
    }

    iterator do_find_linear(const Key& key)
    {
        const iterator it_end = end();
        for (iterator it = begin(); it != it_end; ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return it_end;
    }

    void insertion_sort()
    {
        const iterator end = data_vec_.end();
        const iterator begin = data_vec_.begin();
        for (iterator it = data_vec_.begin(); it != end; ++it)
        {
            const iterator insertion_point = std::upper_bound(begin, it, *it, comperator_);
            // Shifting the unsorted part
            std::rotate(insertion_point, it, it+1);
        }
    }

    void sort()
    {
        std::sort(data_vec_.begin(), data_vec_.end(), comperator_);
        is_sorted_ = true;
    }

    /** Simple helper to test whether the key refers to the last inserted
        element. */
    bool key_is_last_element(const Key& key)
    {
        if (empty()) { return false; }
        return data_vec_.back().first == key;
    }

    void destroy (const_iterator pos)
    {
        data_vec_.erase(pos);
    }

    // --- Member data --
    vector_t    data_vec_;
    comperator  comperator_;
    bool        is_sorted_ = false;

#if (CXX_STANDARD != 98)
#   include "sort_map_s__cpp11.hpp"
#endif
};

} // END namespace estl

#endif // ESTL_SORT_MAP_S_HPP
