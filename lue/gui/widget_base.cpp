#include "widget_base.h"

#include <iostream>

using namespace std;

namespace lue::gui {

widget_base::widget_base(data::data_source_base& data_source, const concepts::uri& path) :
    data_source_(data_source),
    path_       (path)
{

}

widget_base::~widget_base() = default;

widget_base::widget_base(widget_base&&) = default;

void widget_base::on_data_changed(const data::data_source_base& /*data_source*/,
                                  const concepts::uri& /*path*/)
{
    cerr << "ERROR NOT IMPLEMENTED widget_base::on_data_changed\n";
}

} // namespace lue::gui
