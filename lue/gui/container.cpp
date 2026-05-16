#include "container.h"

#include <iostream>
#include "gui/widget_base.h"


// PLATFORM_CPP_INCLUDE_BEGIN
#include "gui/qt/container_impl.h"
// PLATFORM_CPP_INCLUDE_END

using namespace std;


namespace lue::gui {

container::container(data::data_source_base& data_source, const concepts::uri& path) :
    widget_base{data_source, path}
    ,
    // pimpl_{std::make_unique<container_impl>()}
    pimpl_{new container_impl()}
{

}

container::~container()
{
    cerr << "~container() path: "  << path() << "\n";
    delete pimpl_;
}


void container::add_widget(const concepts::uri& path, std::unique_ptr<widget_base> w)
{
    widgets_.emplace(path, std::move(w));
}

} // namespace lue::gui
