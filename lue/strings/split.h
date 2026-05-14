#pragma once

#include <vector>
#include <string>

namespace lue::strings {

std::vector<std::string>        split       (std::string_view s, char delimiter);
std::vector<std::string>        split       (std::string_view s, std::string_view delimiter);
std::vector<std::string_view>   split_view  (std::string_view s, std::string_view delimiter);

} // namespace lue::strings
