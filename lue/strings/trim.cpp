#include "trim.h"
// #include <ranges>

// https://cppscripts.com/trim-in-cpp/
// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string


using namespace std;
using namespace std::string_view_literals;

namespace lue::strings {

std::string trim(std::string_view str)
{
    return rtrim(ltrim(str));
}

string_view trim_view(std::string_view str)
{
    return rtrim_view(ltrim_view(str));
}

std::string ltrim(std::string_view str)
{
    const auto start = str.find_first_not_of(" \t\n");
    return (start == std::string::npos) ? "" : string{str.substr(start)};
}

string_view ltrim_view(std::string_view str)
{
    const auto start = str.find_first_not_of(" \t\n");
    return (start == std::string::npos) ? string_view{} : string_view{str.begin() + start, str.end()};
}

std::string rtrim(std::string_view str)
{
    const auto last = str.find_last_not_of(" \t\n");
    return (last == std::string::npos) ? "" : string{str.substr(0, last + 1)};
}

string_view rtrim_view(std::string_view str)
{
    const auto last = str.find_last_not_of(" \t\n");
    return (last == std::string::npos) ? string_view{} : string_view{str.begin(), str.begin() + last +1};
}

} // namespace lue::strings
