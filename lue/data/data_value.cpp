#include "data_value.h"
#include <ranges>


#include "data_source_base.h"

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
string to_string(const data_value& value)
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
                      }, value);
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



} // namespace lue::data
