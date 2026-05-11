#include "test_page_a1.h"
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <QLabel>

test_page_a1::test_page_a1(lue::data::data_source_base& data_source, QWidget* parent) : page_type_a{data_source, parent}
{
    build_page();
    // build_page_qt_way();
}

void test_page_a1::build_page_qt_way()
{
    setLayout(new QVBoxLayout{});

    QFont f( "Arial", 20, QFont::Bold);
    auto title_lb = new QLabel(data_source().as_string("title").c_str());
    title_lb->setAutoFillBackground(true);
    auto description_lb = new QLabel(data_source().as_string("description").c_str());
    description_lb->setAutoFillBackground(true);
    description_lb->setWordWrap(true);
    QPalette palette = title_lb->palette();
    palette.setColor(QPalette::Window, Qt::black);
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::WindowText, Qt::red);
    // palette.setColor(QPalette::WindowText, Qt::red);
    title_lb->setPalette(palette);


    // QFont font = title_lb->font();
    // font.setPointSize(24);
    // font.setBold(true);
    // title_lb->setFont(font);
    title_lb->setFont(f);
    // title_lb->ba

    layout()->addWidget(title_lb);
    layout()->addWidget(description_lb);
    auto button1 = new QPushButton("Hello from test_page_a1 (qt_way)!");
    layout()->addWidget(button1);
    auto button2 = new QPushButton("Button 2!");
    layout()->addWidget(button2);
    // this->setPalette(palette);

}

void test_page_a1::build_page()
{
    layout_v().label("title").label("description");
    // layout_h().button("H1").button("h2");
}
