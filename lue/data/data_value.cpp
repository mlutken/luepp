#include "data_value.h"
#include <ranges>


using namespace std;
using namespace std::string_view_literals;

namespace lue::data {

// template<class... Ts>
// struct visit_overload_t : Ts... {
//     using Ts::operator()...;
// };
// template<class... Ts>
// visit_overload_t(Ts...) -> visit_overload_t<Ts...>; // Deduction guide

// ----------------
// --- my_point ---
// ----------------

data_object_base::id_t my_point::id() const { return __COUNTER__; }

string my_point::to_string() const
{
    return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}



} // namespace lue::data
