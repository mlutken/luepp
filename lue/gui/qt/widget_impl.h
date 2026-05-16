#pragma once

#include <QWidget>
#include "data/data_source_base.h"

namespace lue::gui {

class widget_impl : public QObject
{
    Q_OBJECT
public:
    explicit widget_impl(QObject* parent = nullptr);

    void on_data_changed    (const data::data_source_base& data_source, const concepts::uri& path);

signals:
private:
};

} // namespace lue::gui
