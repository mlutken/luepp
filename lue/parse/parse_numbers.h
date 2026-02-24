#pragma once
#include <luepp_default_config.h>
#include <cstdint>
#include <charconv>
#include <string>
#include <optional>


namespace lue::parse {



template <typename T>
inline std::optional<T>    num      (std::string_view str)
{
    T val{};
    if (std::from_chars(str.data(), str.data() + str.size(), val).ec == std::errc{}) {
        return val;
    }
    return std::nullopt;
}

} // END namespace lue::parse

