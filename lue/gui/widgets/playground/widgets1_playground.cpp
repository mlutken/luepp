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
using namespace lue::gui::common;


#include <QApplication>
#include <QDebug>




int main(int argc, char *argv[])
{
    cerr << "--- NEW widgets1 playground ---\n";


    auto cleanup = lue::gui::common::app::i().initialize(argc, argv);
    // auto& lue_app = lue::gui::common::app::i();
    // lue::system::core_app::cleanup cleanup;
    // auto lue_app = lue::gui::common::app(argc, argv);

    // c->show();
    QWidget w;
    w.setLayout(new QVBoxLayout{});
    auto button = new QPushButton("Hello world!");
    w.layout()->addWidget(button);
    w.show();



    // lue_app.run();
    app::i().run();
    cerr << "EXIT main()\n";
    // return lue_app.exit_code();
    return app::i().exit_code();
}

