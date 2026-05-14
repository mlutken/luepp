#pragma once

#include <functional>

#include "concepts/uri.h"

namespace lue::data {

using data_path = lue::concepts::uri;

using data_changed_cb = std::function<void (const data_path&)>;

} // namespace lue::data
