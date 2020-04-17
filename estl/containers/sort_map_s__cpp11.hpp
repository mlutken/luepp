public:

explicit sort_map_s(const std::initializer_list<value_type>& init)
    : comperator_(Compare())
{
    for (auto elem : init) {
        data_vec_.push_back(elem);
    }
}

sort_map_s(const std::initializer_list<value_type>& init, const Compare& comperator)
    : comperator_(comperator)
{
    for (auto elem : init) {
        data_vec_.push_back(elem);
    }
}

private:

