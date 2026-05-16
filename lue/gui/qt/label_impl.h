#pragma once

#include <QLabel>
#include "data/data_source_base.h"

namespace lue::gui {

class label_impl : public QObject
{
    Q_OBJECT
public:
    explicit label_impl(QObject* parent = nullptr);

    void on_data_changed    (const data::data_source_base& data_source, const concepts::uri& path);

signals:
private:
    QLabel* q_widget_ptr_{nullptr};
};

} // namespace lue::gui
