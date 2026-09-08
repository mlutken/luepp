#pragma once

#include "gui/widgets/platform_qt6widgets/widget_base_impl.h"

namespace lue::gui::widgets {

class widget_impl : public widget_base_impl
{
    Q_OBJECT
public:
    explicit widget_impl(QObject* parent = nullptr);
    ~widget_impl() override;

signals:
private:
};

} // namespace lue::gui::widgets
