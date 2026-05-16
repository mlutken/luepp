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

private:
    std::unique_ptr<label_impl>     pimpl_;
};

} // namespace lue::gui
