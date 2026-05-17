#pragma once

#include <memory>
#include <unordered_map>
#include "gui/widget_base.h"

namespace lue::gui {

class main_window_impl;

class main_window
{
public:
    explicit main_window();
    ~main_window();

    main_window(const main_window&) = delete;
    main_window& operator=(const main_window&) = delete;
    main_window(main_window&&) = delete;
    main_window& operator=(main_window&&) = delete;

    void        show();

    void        add_widget          (const concepts::uri& path, std::unique_ptr<widget_base> w);

private:
    std::unique_ptr<main_window_impl> pimpl_;
    std::unordered_map<concepts::uri, std::unique_ptr<widget_base>> widgets_;
};

} // namespace lue::gui
