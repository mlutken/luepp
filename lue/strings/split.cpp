#include "split.h"
#include <ranges>

// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c


using namespace std;
using namespace std::string_view_literals;

namespace lue::strings {

std::vector<string> split(std::string_view s, char delimiter)
{
    std::vector<std::string> tokens;
    auto ranges_result = s | std::views::split(delimiter);

    for (auto token : ranges_result) {
        tokens.push_back(std::string(token.begin(), token.end()));
    }

    return tokens;
}

std::vector<string> split(std::string_view s, string_view delimiter)
{
    std::vector<std::string> tokens;
    auto ranges_result = s | std::views::split(delimiter);

    for (auto token : ranges_result) {
        tokens.push_back(std::string(token.begin(), token.end()));
    }

    return tokens;
}


} // namespace lue::strings
