#include "widget_base_impl.h"
// #include "gui/platform_qt6widgets/qt_utils.h"

namespace lue::gui {

widget_base_impl::widget_base_impl(QObject* parent)
    : QObject{parent}
{
}

widget_base_impl::~widget_base_impl() = default;

void widget_base_impl::on_data_changed(const data::data_source_base& /*data_source*/,
                                     const concepts::uri& /*path*/)
{
}

} // namespace lue::gui
