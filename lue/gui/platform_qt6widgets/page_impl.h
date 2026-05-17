#pragma once

#include <QWidget>
#include "gui/widget_base.h"
#include "gui/platform_qt6widgets/widget_base_impl.h"
#include "data/data_source_base.h"

namespace lue::gui {

class page_impl : public widget_base_impl
{
    Q_OBJECT
public:
    explicit page_impl(QObject* parent = nullptr);
    ~page_impl() override;

    void show();
    void on_data_changed    (const data::data_source_base& data_source, const concepts::uri& path) override;
    void add_widget         (widget_base& w);

signals:
private:
    QWidget* q_widget_ptr_{nullptr};
};

} // namespace lue::gui
