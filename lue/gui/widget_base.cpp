#include "widget_base.h"

namespace lue::gui {

widget_base::widget_base(data::data_source_base& data_source, const concepts::uri& path) :
    data_source_(data_source),
    path_       (path)
{

}


} // namespace lue::gui
