#include "data_source_memory.h"
#include <ranges>
#include <sstream>
#include <iostream>

#include "data/data_value.h"

using namespace std;
using namespace std::string_view_literals;

namespace lue::data {


data_source_base::data_source_base(data_path path) :
    path_{path}
{
}


void data_source_base::set(const data_path& path, data_value val)
{
    do_set(path, std::move(val));
    call_on_changed(path);
}

void data_source_base::set(string_view path, data_value val)
{
    set(data_path{path}, std::move(val));
}

void data_source_base::set(const data_path& path, data_value_vec val)
{
    do_set_data_vec(path, std::move(val));
    call_on_changed(path);
}

void data_source_base::set(std::string_view path, data_value_vec val)
{
    set(data_path{path}, std::move(val));
}

void data_source_base::set_data_source(const data_path& path, data_source_base_sp val)
{
    do_set(path, std::move(val));
    call_on_changed(path);
}

int32_t data_source_base::as_int32(const data_path& path) const
{
    return std::get<int32_t>(as_data_value(path));
}

const string& data_source_base::as_string(const data_path& path) const
{
    return std::get<std::string>(as_data_value(path));
}

const data_value& data_source_base::as_data_value(const data_path& path) const
{
    return do_as_data_value(path);
}

void data_source_base::connect(data_changed_cb cb)
{
    data_changed_cb_vec_.push_back(std::move(cb));
}

bool data_source_base::is_read_only() const
{
    return do_is_read_only();
}

string data_source_base::to_string() const
{
    return do_to_string();
}

void data_source_base::dbg_print() const
{
    cerr << to_string() << "\n";
}

void data_source_base::call_on_changed(const data_path& path) const
{
    for (const auto& cb: data_changed_cb_vec_) {
        cb(path);
    }
}


} // namespace lue::data
