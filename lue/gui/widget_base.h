#pragma once

#include "data/data_source_base.h"

namespace lue::gui {
class widget_base
{
public:
    explicit widget_base(data::data_source_base& data_source, const concepts::uri& path);
    virtual ~widget_base() = default;


// private:
protected:
    data::data_source_base&         data_source_;
    concepts::uri                   path_;

private:

};

} // namespace lue::gui
