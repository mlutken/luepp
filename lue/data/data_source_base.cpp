#include "data_source.h"
#include <ranges>
#include <sstream>
#include <iostream>

#include "data/data_value.h"

using namespace std;
using namespace std::string_view_literals;

namespace lue::data {

template<class... Ts>
struct visit_overload_t : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
visit_overload_t(Ts...) -> visit_overload_t<Ts...>; // Deduction guide


// ------------------
// --- data_value ---
// ------------------

/// @see https://www.cppstories.com/2018/09/visit-variants/
string to_string(const data_value& val)
{
    return std::visit(visit_overload_t {
                          [](const std::string& b)            -> string {
                              return b;                       },
                          [](bool val)                        -> string {
                              return val ? "true" : "false";  },
                          [](const data_object_base& val)     -> string {
                              return val.to_string();         },
                          [](const data_object_sp& val)       -> string {
                              return val->to_string();        },
                          [](const data_source_base& val)     -> string {
                              return val.to_string();         },
                          [](const data_source_base_sp& val)  -> string {
                              return val->to_string();        },
                          [](auto val)                        -> string {
                              return std::to_string(val);      },
                      }, val);
}

string to_string(const data_value_vec& vec)
{
    string s{};
    for (const auto& val: vec | views::take(vec.size()-1)) {
        s += to_string(val) + ", ";
    }
    s += to_string(vec.back());
    return s;
}


data_source_base::data_source_base(data_path path) :
    path_{path}
{
}

void data_source_base::set(const data_path& path, data_value val)
{
    do_set(path, std::move(val));
    call_on_changed(path);
}

void data_source_base::set(const data_path& path, data_value_vec val)
{
    do_set_data_vec(path, std::move(val));
    call_on_changed(path);
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
