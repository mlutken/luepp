#include "label_impl.h"
#include "gui/qt/qt_utils.h"

namespace lue::gui {

label_impl::label_impl(QObject* parent)
    : QObject{parent},
    q_widget_ptr_{new QLabel{}}
{
    // q_widget_ptr_->setAlignment(Qt::AlignCenter);
}

void label_impl::on_data_changed(const data::data_source_base& data_source,
                                 const concepts::uri& path)
{
    // q_widget_ptr_->setText(QString::fromStdString(data_source.as_string(path)));
    q_widget_ptr_->setText(to_qstring(data_source.as_string(path)));
}

} // namespace lue::gui
