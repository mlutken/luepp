#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <variant>

#include "data/data_object_base.h"

namespace lue::data {

// ------------------
// --- data_value ---
// ------------------
class data_source_base;
using data_source_base_sp   = std::shared_ptr<data_source_base>;

using data_value            = std::variant<int32_t, int64_t, float, double, bool, std::string,
                                    data_object_sp, data_source_base_sp>;
using data_value_vec        = std::vector<data_value>;

std::string     to_string   (const data_value& val);
std::string     to_string   (const data_value_vec& val);



} // namespace lue::data
