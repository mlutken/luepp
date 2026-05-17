#pragma once

#include <QMainWindow>
#include "gui/widget_base.h"
#include "gui/platform_qt6widgets/widget_base_impl.h"

namespace lue::gui {

class main_window_impl : public widget_base_impl
{
    Q_OBJECT
public:
    explicit main_window_impl(QObject* parent = nullptr);
    ~main_window_impl() override;

    void set_widget(QWidget* q_widget_ptr);
    void show();
    void on_data_changed    (const data::data_source_base& data_source, const concepts::uri& path) override;
    void add_widget         (widget_base& w);

signals:
private:
    QMainWindow* q_widget_ptr_{nullptr};
};

} // namespace lue::gui
