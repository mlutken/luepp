#pragma once
#include <memory>

#include "data/data_source_base.h"
#include "gui/widget_base.h"

namespace lue::gui {

class label_impl;

class label : public widget_base
{
public:
    explicit label(data::data_source_base& data_source, const concepts::uri& path);
    ~label() override;
    // Disable copy and move assignment operators.
    label(const label&) = delete;
    label& operator=(const label&) = delete;
    label(label&&);
    label& operator=(label&&) = delete;

private:
    std::unique_ptr<label_impl>     pimpl_;
};

} // namespace lue::gui
