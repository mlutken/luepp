#include "container.h"

#include <iostream>
#include "gui/widget_base.h"


// PLATFORM_CPP_INCLUDE_BEGIN
#include "gui/platform_qt6widgets/container_impl.h"
// PLATFORM_CPP_INCLUDE_END

using namespace std;


namespace lue::gui {

container::container(data::data_source_base& data_source, const concepts::uri& path) :
    widget_base{data_source, path}
    ,
    pimpl_{std::make_unique<container_impl>()}
{

}

container::~container()
{
    cerr << "~container() path: " << path() << "\n";
    // delete pimpl_;
}

container::container(container&&) = default;

void container::show()
{
    pimpl_->show();
}

void container::add_widget(const concepts::uri& path, std::unique_ptr<widget_base> w)
{
    widgets_.emplace(path, std::move(w));
    widget_base& widget = *widgets_.at(path);
    pimpl_->add_widget(widget);
}

} // namespace lue::gui
