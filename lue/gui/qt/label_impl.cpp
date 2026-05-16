#include "label_impl.h"
#include "gui/qt/qt_utils.h"

namespace lue::gui {

label_impl::label_impl(QObject* parent)
    : widget_base_impl{parent},
    q_widget_ptr_{new QLabel{}}
{
    // q_widget_ptr_->setAlignment(Qt::AlignCenter);
}

label_impl::~label_impl() = default;

void label_impl::on_data_changed(const data::data_source_base& data_source,
                                 const concepts::uri& path)
{
    // q_widget_ptr_->setText(QString::fromStdString(data_source.as_string(path)));
    q_widget_ptr_->setText(to_qstring(data_source.as_string(path)));
}

QWidget* label_impl::widget()
{
    return q_widget_ptr_;
}

} // namespace lue::gui
