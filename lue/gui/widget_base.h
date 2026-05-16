#pragma once

#include "data/data_source_base.h"

namespace lue::gui {
class widget_base
{
public:
    explicit widget_base(data::data_source_base& data_source, const concepts::uri& path);
    virtual ~widget_base();

    widget_base(const widget_base&) = delete;
    widget_base& operator=(const widget_base&) = delete;
    widget_base(widget_base&&);
    widget_base& operator=(widget_base&&) = delete;

    const data::data_source_base&       data_source     () const    { return data_source_; }
    data::data_source_base&             data_source     ()          { return data_source_; }
    const concepts::uri&                path            () const    { return path_; }
private:
    data::data_source_base&         data_source_;
    concepts::uri                   path_;

private:

};

} // namespace lue::gui
