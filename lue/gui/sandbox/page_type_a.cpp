#include "page_type_a.h"
#include <iostream>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qpushbutton.h>



template<class... Ts>
struct visit_overload_t : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
visit_overload_t(Ts...) -> visit_overload_t<Ts...>; // Deduction guide

using namespace std;

page_type_a::page_type_a(lue::data::data_source_base& data_source, QWidget* parent)
    : QWidget{parent},
      data_source_ref_ptr_{&data_source}
{
    this->data_source().connect([this](const lue::data::data_path& path) { on_data_changed(path);});
}

page_type_a& page_type_a::layout_h()
{
    add_layout_helper(new QHBoxLayout{});
    return *this;
}

page_type_a &page_type_a::layout_v()
{
    add_layout_helper(new QVBoxLayout{});
    return *this;
}

page_type_a& page_type_a::layout_pop()
{
    add_layout_helper(new QVBoxLayout{});
    return *this;
}

page_type_a& page_type_a::label(const lue::data::data_path& path)
{
    auto label = new QLabel(QString::fromUtf8(data_source().as_string(path).c_str()));
    label->setWordWrap(true);
    add_widget_helper(label);
    widget_map_[path] = label;
    return *this;
}

page_type_a& page_type_a::button(const lue::data::data_path& path)
{
    auto button = new QPushButton(QString::fromUtf8(data_source().as_string(path).c_str()));
    add_widget_helper(button);
    widget_map_[path] = button;
    return *this;
}


lue::data::data_source_base& page_type_a::data_source()
{
    if (data_source_ref_ptr_) { return *data_source_ref_ptr_; }
    return *data_source_sp_;
}

const lue::data::data_source_base& page_type_a::data_source() const
{
    if (data_source_ref_ptr_) { return *data_source_ref_ptr_; }
    return *data_source_sp_;
}

void page_type_a::on_data_changed(const lue::data::data_path& path)
{
    cerr << "FIXMENM on_data_changed: " << path.string() << "\n";
}

// -----------------------
// --- PRIVATE helpers ---
// -----------------------

void page_type_a::add_layout_helper(QLayout* layout)
{
    if (!has_cur_layout()) {
        setLayout(layout);
        layout_stack_.push(layout);
        return;
    }

    if (auto* box_layout = dynamic_cast<QBoxLayout*>(cur_layout()); box_layout != nullptr) {
        box_layout->addLayout(layout);
    }
    layout_stack_.push(layout);
}

void page_type_a::add_widget_helper(QWidget *w)
{
    cur_layout()->addWidget(w);
}

QLayout* page_type_a::cur_layout() const
{
    if (layout_stack_.empty()) {
        return nullptr;
    }
    return layout_stack_.top();
}

bool page_type_a::has_cur_layout() const
{
    return cur_layout() != nullptr;
}

QLayout *page_type_a::default_layout()
{
    return new QVBoxLayout{};
}
