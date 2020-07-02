public:

vector_s( vector_s&& other ) noexcept
{
    for (auto i = other.size(); i > 0; ) {
        --i;
        const pointer insert_ptr = data_ptr_ + i;
        new (insert_ptr) value_type(NESTLE_MOVE(other[i]));
    }
    size_ = other.size();
}

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
        const pointer insert_ptr = data_ptr_ + i;
        new (insert_ptr) value_type(*it); // TODO: Is it OK to move here! Elements are already copied into initializer_list?
    }
    size_ = diff;
}

vector_s& operator=(vector_s&& other)
{
    // NOTE: The reinterpret cast is needed as the pointer types can potentially
    //       be different if the capacities of the vectors are different.
    //       As we merely need to check the pointer adresses it's ok with
    //       with this "crude" cast here!
    if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) // prevent self-assignment
    {
        for (auto i = other.size(); i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(NESTLE_MOVE(other[i]));
        }
        size_ = other.size();
    }
    return *this;
}

template<size_t CAPACITY_OTHER>
vector_s& operator=(vector_s<T, CAPACITY_OTHER>&& other)
{
    // NOTE: The reinterpret cast is needed as the pointer types can potentially
    //       be different if the capacities of the vectors are different.
    //       As we merely need to check the pointer adresses it's ok with
    //       with this "crude" cast here!
    if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) // prevent self-assignment
    {
        for (auto i = other.size(); i > 0; ) {
            --i;
            const pointer insert_ptr = data_ptr_ + i;
            new (insert_ptr) value_type(NESTLE_MOVE(other[i]));
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
        const pointer insert_ptr = data_ptr_ + i;
        new (insert_ptr) value_type(*it); // TODO: Is it OK to move here! Elements are already copied into initializer_list?
    }
    size_ = diff;
    return *this;
}

void assign(std::initializer_list<T> ilist)
{
    auto it = ilist.begin();
    const auto end = ilist.end();
    const auto diff = check_range(it, end);
    size_type i = 0u;
    for (; it != end; ++it, ++i) {
        const pointer insert_ptr = data_ptr_ + i;
        new (insert_ptr) value_type(*it);    // TODO: Is it OK to move here! Elements are already copied into initializer_list?
    }
    size_ = diff;
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
        ESTL_THROW (std::range_error, "cas::vector_s emplace beyond capacity.");
        return end();
    }

    iterator ipos = const_cast<iterator>(pos);
    shift_right(ipos, end(), 1u);
    new (static_cast<pointer>(ipos)) T(NESTLE_FORWARD(args)...);
    size_ = new_size;
    return ipos;
}

void push_back (T&& value)
{
    const auto new_size = size() + 1;
    if (new_size > capacity()) {
        ESTL_THROW(std::range_error, "cas::vector_s push_back beyond capacity.");
    }
    const pointer insert_ptr = data_ptr_ + size();
    new (insert_ptr) value_type{NESTLE_MOVE(value)};
    size_ = new_size;
}


template<class... Args >
reference emplace_back(Args&&... args)
{
    const auto new_size = size() + 1;
    if (new_size > capacity()) {
        ESTL_THROW (std::range_error, "cas::vector_s emplace_back beyond capacity.");
    }

    const pointer insert_ptr = data_ptr_ + size();
    new (insert_ptr) value_type(NESTLE_FORWARD(args)...);
    size_ = new_size;
    return *insert_ptr;
}

iterator insert(const_iterator pos, const T&& value)
{
    const auto new_size = size() + 1u;
    if (new_size > capacity()) {
        ESTL_THROW (std::range_error, "cas::vector_s inserting beyond capacity.");
        return end();
    }

    iterator ipos = const_cast<iterator>(pos);
    shift_right(ipos, end(), 1u);// Allocates new elements

    const pointer insert_ptr = static_cast<pointer>(ipos);
    new (insert_ptr) value_type(NESTLE_MOVE(value));
    // *ipos = NESTLE_MOVE(value); // TODO: This seems ok too since the extra elements are already "allocated" by shift_right

    size_ = new_size;
    return ipos;
}
private:

