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


#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    cerr << "--- NEW widgets1 playground ---\n";


    lue::gui::common::app lue_app{argc, argv};

    // c->show();
    QWidget w;
    w.setLayout(new QVBoxLayout{});
    auto button = new QPushButton("Hello world!");
    w.layout()->addWidget(button);
    w.show();


    QWidget w2;
    w2.setLayout(new QVBoxLayout{});
    auto button2 = new QPushButton("Hello world 2!");
    w2.layout()->addWidget(button2);
    w2.show();

    // return a->start(); // .exec starts QApplication and related GUI, this line starts 'event loop'
    return lue_app.start();
}

