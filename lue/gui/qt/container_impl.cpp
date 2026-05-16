#include "container_impl.h"
// #include "gui/qt/qt_utils.h"

namespace lue::gui {

container_impl::container_impl(QObject* parent)
    : QObject{parent},
    q_widget_ptr_{new QWidget{}}
{
}

void container_impl::on_data_changed(const data::data_source_base& /*data_source*/,
                                     const concepts::uri& /*path*/)
{
}

} // namespace lue::gui
