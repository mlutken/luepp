#pragma once

#include <QWidget>
#include "gui/widget_base.h"
#include "gui/qt/widget_base_impl.h"
#include "data/data_source_base.h"

namespace lue::gui {

class container_impl : public widget_base_impl
{
    Q_OBJECT
public:
    explicit container_impl(QObject* parent = nullptr);
    ~container_impl() override;

    void set_widget(QWidget* q_widget_ptr);
    void show();
    void on_data_changed    (const data::data_source_base& data_source, const concepts::uri& path) override;
    void add_widget         (widget_base& w);

signals:
private:
    QWidget* q_widget_ptr_{nullptr};
};

} // namespace lue::gui
