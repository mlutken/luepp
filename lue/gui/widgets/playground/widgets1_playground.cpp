#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <gui/widgets/label.h>
#include <gui/common/app.h>

// Web resources
// Understanding QWindow vs. QWidget for Sizing: https://runebook.dev/en/docs/qt/qwindow/minimumHeight-prop
using namespace std;
using namespace lue::gui::widgets;




int main(int argc, char *argv[])
{
    cerr << "--- widgets1 playground ---\n";


    // lue::gui::app lue_app;
    // lue::gui::common::hest lue_app{};
    // lue::gui::app a(argc, argv);

    QApplication a(argc, argv);
    
    // lue::data::data_source_memory ds1 = create_demo_1();
    // // cerr << "FIXMENM ds1.description: " << ds1.as_string("description"_uri) << "\n";
    // test_page_a1 w(ds1);

    // // auto c = create_demo_3(ds1);
    // auto c = create_demo_2(ds1);

    // c->show();
    QWidget w;
    w.setLayout(new QVBoxLayout{});
    auto button = new QPushButton("Hello world!");
    w.layout()->addWidget(button);

    w.show();

    return a.exec(); // .exec starts QApplication and related GUI, this line starts 'event loop'
    // return a.start();
}

