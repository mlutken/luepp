#include "widget_impl.h"
// #include "gui/platform_qt6widgets/qt_utils.h"

namespace lue::gui {

widget_impl::widget_impl(QObject* parent)
    : widget_base_impl{parent}
{
}

widget_impl::~widget_impl() = default;

void widget_impl::on_data_changed(const data::data_source_base& /*data_source*/,
                                     const concepts::uri& /*path*/)
{
}

} // namespace lue::gui
