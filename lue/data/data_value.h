#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <variant>

#include "data/data_source_base.h"
#include "data/data_object_base.h"

namespace lue::data {

// ----------------
// --- my_point ---
// ----------------

struct my_point : public data_object_base
{
    my_point() = default;
    ~my_point() override = default;
    my_point(const my_point&) = default;
    my_point(my_point&&) = default;
    my_point& operator=(const my_point&) = default;
    my_point& operator=(my_point&&) = default;
    
    explicit my_point(uint32_t xval, uint32_t yval) : x(xval), y(yval) {}
    
    uint32_t x  {};
    uint32_t y  {};
    id_t            id          () const override;
    std::string     to_string   () const override;
    
};



} // namespace lue::data
