#include "label_impl.h"
#include "gui/platform_qt6widgets/qt_utils.h"

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

void label_impl::text(const std::string& text)
{
    q_widget_ptr_->setText(to_qstring(text));
}

void label_impl::word_wrap(bool wrap)
{
    q_widget_ptr_->setWordWrap(wrap);
}

QWidget* label_impl::widget()
{
    return q_widget_ptr_;
}

} // namespace lue::gui
