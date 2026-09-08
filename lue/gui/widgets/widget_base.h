#pragma once

#include <gui/widgets/layoutable.h>

namespace lue::gui::widgets {
class widget_base_impl;

class widget_base : public layoutable
{
public:
    widget_base () = default;
    widget_base (widget_base&&) = default;
    widget_base (const widget_base&) = delete;
    widget_base& operator=(const widget_base&) = delete;
    widget_base& operator=(widget_base&&) = delete;

    // TODO: Make these pure virtual
    virtual widget_base_impl* impl() { return nullptr; }
    // virtual const widget_base_impl*     impl            () const { return nullptr; }

private:

};

} // namespace lue::gui::widgets
