#pragma once

#include <cstdint>


namespace lue::concepts {

enum class access_mode : std::uint8_t
{
    read_only,
    read_write
};

} // namespace lue::concepts
