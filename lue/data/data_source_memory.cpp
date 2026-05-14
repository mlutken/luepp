#include "data_source_memory.h"
#include <ranges>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::string_view_literals;

namespace lue::data {

// template<class... Ts>
// struct visit_overload_t : Ts... {
//     using Ts::operator()...;
// };
// template<class... Ts>
// visit_overload_t(Ts...) -> visit_overload_t<Ts...>; // Deduction guide

// // ----------------
// // --- my_point ---
// // ----------------

// data_object_base::id_t my_point::id() const { return __COUNTER__; }

// string my_point::to_string() const
// {
//     return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
// }

// ------------------
// --- data_value ---
// ------------------

// /// @see https://www.cppstories.com/2018/09/visit-variants/
// string to_string(const data_value& val)
// {
//     return std::visit(visit_overload_t {
//         [](const std::string& b)            -> string { return b;                       },
//         [](bool val)                        -> string { return val ? "true" : "false";  },
//         [](const data_object_base& val)     -> string { return val.to_string();         },
//         [](const data_object_sp& val)       -> string { return val->to_string();        },
//         [](const data_source_base& val)     -> string { return val.to_string();         },
//         [](const data_source_base_sp& val)  -> string { return val->to_string();        },
//         [](auto val)                        -> string { return to_string(val);          },
//     }, val);
// }

// string to_string(const data_value_vec& vec)
// {
//     string s{};
//     for (const auto& val: vec | views::take(vec.size()-1)) {
//         s += to_string(val) + ", ";
//     }
//     s += to_string(vec.back());
//     return s;
// }

// -------------------
// --- data_source ---
// -------------------

const data_value& data_source_memory::do_as_data_value(const data_path& path) const
{
    static const data_value default_value{""s};
    const auto it = string_data_map_.find(path);
    if (it == string_data_map_.end()) {
        return default_value;
    }
    return it != string_data_map_.end() ? it->second : default_value;
}

void data_source_memory::do_set(const data_path& path, data_value val)
{
    string_data_map_[path] = std::move(val);
}

void data_source_memory::do_set_data_vec(const data_path& path, data_value_vec val)
{
    string_vec_map_[path] = std::move(val);
}

void data_source_memory::do_set_data_source(const data_path& path, data_source_base_sp val)
{
    string_data_map_[path] = std::move(val);
}

string data_source_memory::do_to_string() const
{
    string s;
    s += "{" + path().string() + "}\n";
    for (const auto& [path, val]: string_data_map_) {
        s += path.string() + " : ";
        s += lue::data::to_string(val) + "\n";
    }
    for (const auto& [path, vec]: string_vec_map_) {
        s += "[" + path.string() + "]: ";
        s += lue::data::to_string(vec) + "\n";
    }

    return s;
}


} // namespace lue::data
