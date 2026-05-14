#pragma once
#include <memory>

#include "data/data_source_base.h"
#include "gui/widget.h"

namespace lue::gui {

class label : public widget
{
public:
    explicit label(data::data_source_base& data_source, const concepts::uri& path);

private:
    class impl;
    data::data_source_base& data_source_;
    concepts::uri           path_;
    std::unique_ptr<impl>   pimpl_;
};

} // namespace lue::gui
