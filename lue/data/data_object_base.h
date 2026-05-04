#pragma once

#include <string>
#include <memory>


// ------------------------
// --- data_object_base ---
// ------------------------

namespace lue::data {

struct data_object_base
{
    using id_t = size_t;
    virtual ~data_object_base() = default;
    virtual id_t            id          () const = 0;
    virtual std::string     to_string   () const = 0;
};

using data_object_sp = std::shared_ptr<data_object_base>;


} // namespace lue::data
