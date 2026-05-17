#include "page_impl.h"

#include <QLayout>
#include <QLabel>   // TODO: Remove

// #include "gui/platform_qt6widgets/qt_utils.h"

namespace lue::gui {

page_impl::page_impl(QObject* parent)
    : widget_base_impl{parent}
    ,
    q_widget_ptr_{new QWidget{}}
{
    q_widget_ptr_->setLayout(new QVBoxLayout{});
}

page_impl::~page_impl() = default;

// QWidget* page_impl::widget()
// {
//     return q_widget_ptr_.get();
// }

void page_impl::show()
{
    q_widget_ptr_->show();
}

void page_impl::on_data_changed(const data::data_source_base& /*data_source*/,
                                     const concepts::uri& /*path*/)
{
}

void page_impl::add_widget(widget_base& w)
{
    // q_widget_ptr_->layout()->addWidget(new QLabel{"HEST"});
    q_widget_ptr_->layout()->addWidget(w.impl()->widget());
}

} // namespace lue::gui
