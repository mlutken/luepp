#include "container_impl.h"

#include <QLayout>
#include <QLabel>   // TODO: Remove

// #include "gui/platform_qt6widgets/qt_utils.h"

namespace lue::gui {

container_impl::container_impl(QObject* parent)
    : widget_base_impl{parent}
    ,
    q_widget_ptr_{new QWidget{}}
{
    q_widget_ptr_->setLayout(new QVBoxLayout{});
}

container_impl::~container_impl() = default;

// QWidget* container_impl::widget()
// {
//     return q_widget_ptr_.get();
// }

void container_impl::show()
{
    q_widget_ptr_->show();
}

void container_impl::on_data_changed(const data::data_source_base& /*data_source*/,
                                     const concepts::uri& /*path*/)
{
}

void container_impl::add_widget(widget_base& w)
{
    // q_widget_ptr_->layout()->addWidget(new QLabel{"HEST"});
    q_widget_ptr_->layout()->addWidget(w.impl()->widget());
}

} // namespace lue::gui
