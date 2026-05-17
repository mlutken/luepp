#pragma once

#include <QLabel>
#include "gui/platform_qt6widgets/widget_base_impl.h"
#include "data/data_source_base.h"

namespace lue::gui {

class label_impl : public widget_base_impl
{
    Q_OBJECT
public:
    explicit label_impl(QObject* parent = nullptr);
    ~label_impl() override;

    void            on_data_changed    (const data::data_source_base& data_source, const concepts::uri& path) override;

    void            text                (const std::string& text);
    void            word_wrap           (bool wrap);

    QWidget*        widget              () override;

signals:
private:
    QLabel* q_widget_ptr_{nullptr};
};

} // namespace lue::gui
