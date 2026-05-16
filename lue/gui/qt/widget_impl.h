#pragma once

#include "gui/qt/widget_base_impl.h"
#include "data/data_source_base.h"

namespace lue::gui {

class widget_impl : public widget_base_impl
{
    Q_OBJECT
public:
    explicit widget_impl(QObject* parent = nullptr);
    ~widget_impl() override;

    void on_data_changed    (const data::data_source_base& data_source, const concepts::uri& path) override;

signals:
private:
};

} // namespace lue::gui
