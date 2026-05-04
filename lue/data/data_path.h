#pragma once

#include <string>
#include <functional>

namespace lue::data {

/// @todo Make real class using std::filesystem::path internally. Must support network paths as well.
using data_path = std::string;

using data_changed_cb = std::function<void (const data_path&)>;

} // namespace lue::data
