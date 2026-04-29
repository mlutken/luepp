#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "gui/sandbox/test_page_a1.h"
#include "data/data_source.h"


using namespace std;


int main(int argc, char *argv[])
{
    cerr << "--- qt1 playground ---\n";


    QApplication a(argc, argv);

    data_source ds1 = create_demo_1();
    test_page_a1 w(ds1);

    // QWidget w;
    // w.setLayout(new QVBoxLayout{});
    // auto button = new QPushButton("Hello world!");
    // w.layout()->addWidget(button);

    w.show();

    return a.exec(); // .exec starts QApplication and related GUI, this line starts 'event loop'
}
