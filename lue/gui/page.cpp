#include "page.h"

#include <iostream>
#include "gui/widget_base.h"


// PLATFORM_CPP_INCLUDE_BEGIN
#include "gui/platform_qt6widgets/page_impl.h"
// PLATFORM_CPP_INCLUDE_END

using namespace std;


namespace lue::gui {

page::page(data::data_source_base& data_source, const concepts::uri& path) :
    container{data_source, path},
    pimpl_{std::make_unique<page_impl>()}
{

}

page::~page()
{
    cerr << "~page() path: " << path() << "\n";
    // delete pimpl_;
}

page::page(page&&) = default;

void page::show()
{
    pimpl_->show();
}

void page::add_widget(const concepts::uri& path, std::unique_ptr<widget_base> w)
{
    widgets_.emplace(path, std::move(w));
    widget_base& widget = *widgets_.at(path);
    pimpl_->add_widget(widget);
}

} // namespace lue::gui
