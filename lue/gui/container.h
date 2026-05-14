#pragma once

#include <memory>
// #include "gui/widget.h"

namespace lue::gui {

class container
{
public:
    container();
private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

} // namespace lue::gui
