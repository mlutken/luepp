#pragma once

#include <math/base/v2.hpp>

namespace lue::gui::layout {

// TODO: Inspiration: /usr/include/x86_64-linux-gnu/qt6/QtWidgets/qlayoutitem.h
class layoutable
{
public:
    layoutable();
    virtual ~layoutable() = default;

};

} // namespace lue::gui::layout
