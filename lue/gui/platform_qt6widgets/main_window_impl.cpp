#include "main_window_impl.h"

#include <QLayout>
#include <QLabel>   // TODO: Remove

// #include "gui/platform_qt6widgets/qt_utils.h"

namespace lue::gui {

main_window_impl::main_window_impl(QObject* parent)
    : widget_base_impl{parent}
    ,
    q_widget_ptr_{new QMainWindow{}}
{
    q_widget_ptr_->setLayout(new QVBoxLayout{});
}

main_window_impl::~main_window_impl() = default;

// QWidget* main_window_impl::widget()
// {
//     return q_widget_ptr_.get();
// }

void main_window_impl::show()
{
    q_widget_ptr_->show();
}

void main_window_impl::on_data_changed(const data::data_source_base& /*data_source*/,
                                     const concepts::uri& /*path*/)
{
}

void main_window_impl::add_widget(widget_base& w)
{
    // q_widget_ptr_->layout()->addWidget(new QLabel{"HEST"});
    q_widget_ptr_->layout()->addWidget(w.impl()->widget());
}

} // namespace lue::gui
