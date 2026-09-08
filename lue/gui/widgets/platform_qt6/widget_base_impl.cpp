#include "widget_base_impl.h"

namespace lue::gui::widgets {

widget_base_impl::widget_base_impl(QObject* parent)
    : QObject{parent}
{
}

widget_base_impl::~widget_base_impl() = default;


} // namespace lue::gui::widgets
