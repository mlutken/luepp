#pragma once

#include <QLabel>
#include "gui/widgets/platform_qt6/widget_base_impl.h"

namespace lue::gui::widgets {

class label_impl : public widget_base_impl
{
    Q_OBJECT
public:
    explicit label_impl(QObject* parent = nullptr);
    ~label_impl() override;

    void            text                (const std::string& text);
    void            word_wrap           (bool wrap);

    QWidget*        widget              () override;

signals:
private:
    QLabel* q_widget_ptr_{nullptr};
};

} // namespace lue::gui::widgets
