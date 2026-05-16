#include "widget_impl.h"
// #include "gui/qt/qt_utils.h"

namespace lue::gui {

widget_impl::widget_impl(QObject* parent)
    : QObject{parent}
{
}

void widget_impl::on_data_changed(const data::data_source_base& /*data_source*/,
                                     const concepts::uri& /*path*/)
{
}

} // namespace lue::gui
