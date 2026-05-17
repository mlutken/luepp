#include "main_window.h"

#include <iostream>
#include "gui/widget_base.h"


// PLATFORM_CPP_INCLUDE_BEGIN
#include "gui/qt/main_window_impl.h"
// PLATFORM_CPP_INCLUDE_END

using namespace std;


namespace lue::gui {

main_window::main_window() :
    pimpl_{std::make_unique<main_window_impl>()}
{

}

main_window::~main_window() = default;


void main_window::show()
{
    pimpl_->show();
}

void main_window::add_widget(const concepts::uri& path, std::unique_ptr<widget_base> w)
{
    widgets_.emplace(path, std::move(w));
    widget_base& widget = *widgets_.at(path);
    pimpl_->add_widget(widget);
}

} // namespace lue::gui
