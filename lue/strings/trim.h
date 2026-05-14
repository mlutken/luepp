#pragma once

#include <string>

namespace lue::strings {

std::string         trim            (std::string_view str);
std::string_view    trim_view       (std::string_view str);
std::string         ltrim           (std::string_view str);
std::string_view    ltrim_view      (std::string_view str);
std::string         rtrim           (std::string_view str);
std::string_view    rtrim_view      (std::string_view str);

} // namespace lue::strings
