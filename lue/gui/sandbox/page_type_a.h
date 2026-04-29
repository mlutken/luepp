#pragma once

#include <QWidget>
#include <unordered_map>
#include <stack>
#include <variant>

#include "data/data_source.h"

class data_source_base;

class QLabel;
class QButton;
class QVBoxLayout;
class QVBoxLayout;
class QHBoxLayout;

class page_type_a : public QWidget
{
    Q_OBJECT
public:
    explicit page_type_a(data_source_base& data_source, QWidget* parent = nullptr);

    page_type_a&                layout_h    ();
    page_type_a&                layout_v    ();
    page_type_a&                layout_pop  ();

    page_type_a&                label       (const data_path& path);
    page_type_a&                button      (const data_path& path);

    data_source_base&           data_source ();
    const data_source_base&     data_source () const;

signals:
private:
    using widget_variant    = std::variant<QWidget*, QButton*, QLabel*>;
    using layout_stack_t    = std::stack<QLayout*>;
    using widget_map_t      = std::unordered_map<data_path, widget_variant>;

    void                on_data_changed     (const data_path& path);

    void                add_layout_helper   (QLayout* layout);
    // void                cur_layout          ();
    void                add_widget_helper   (QWidget* w);
    QLayout*            cur_layout          () const;
    bool                has_cur_layout      () const;
    static QLayout*     default_layout      ();


    data_source_base*           data_source_ref_ptr_    {nullptr};
    data_source_base_sp         data_source_sp_         {nullptr};

    layout_stack_t      layout_stack_       {};
    widget_map_t        widget_map_         {};
};
