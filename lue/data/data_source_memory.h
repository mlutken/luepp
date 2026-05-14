#pragma once

#include <cstdint>
#include <string>
#include <vector>
// #include <memory>
#include <unordered_map>
// #include <variant>
// #include <functional>

#include "data/data_path.h"
// #include "data/data_object_base.h"
#include "data/data_source_base.h"
#include "data/data_value.h"

// ------------------------
// --- data_object_base ---
// ------------------------

namespace lue::data {


class data_source_memory : public data_source_base
{
public:
    using data_source_base::data_source_base;
    data_source_memory() = default;


protected:
    const data_value&           do_as_data_value    (const data_path& path)  const override;
    void                        do_set              (const data_path& path, data_value val) override;
    void                        do_set_data_vec     (const data_path& path, data_value_vec val) override;
    void                        do_set_data_source  (const data_path& path, data_source_base_sp val) override;

    std::string                 do_to_string       () const override;

private:
    using map_string_data_t     = std::unordered_map<data_path, data_value>;
    using map_string_vec_t      = std::unordered_map<data_path, data_value_vec>;

    map_string_data_t           string_data_map_        {};
    map_string_vec_t            string_vec_map_         {};
};

} // namespace lue::data
