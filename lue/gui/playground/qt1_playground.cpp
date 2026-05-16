#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "gui/sandbox/test_page_a1.h"
#include "data/data_source_memory.h"
#include "gui/label.h"
#include "gui/container.h"

// Web resources
// Understanding QWindow vs. QWidget for Sizing: https://runebook.dev/en/docs/qt/qwindow/minimumHeight-prop
using namespace std;
using namespace lue::litterals;
using namespace lue::gui;

// lue::gui::container create_demo_3(lue::data::data_source_base& data_source);
std::unique_ptr<lue::gui::container> create_demo_2(lue::data::data_source_base& data_source);
lue::data::data_source_memory create_demo_1();



int main(int argc, char *argv[])
{
    cerr << "--- qt1 playground ---\n";


    QApplication a(argc, argv);
    
    lue::data::data_source_memory ds1 = create_demo_1();
    // cerr << "FIXMENM ds1.description: " << ds1.as_string("description"_uri) << "\n";
    test_page_a1 w(ds1);

    // auto c = create_demo_3(ds1);
    auto c = create_demo_2(ds1);

    c->show();
    // QWidget w;
    // w.setLayout(new QVBoxLayout{});
    // auto button = new QPushButton("Hello world!");
    // w.layout()->addWidget(button);

    // w.show();

    return a.exec(); // .exec starts QApplication and related GUI, this line starts 'event loop'
}


std::unique_ptr<lue::gui::container> create_demo_2(lue::data::data_source_base& data_source)
{
    using namespace lue::gui;
    auto c = make_unique<container>(data_source, "/"_uri);
    std::unique_ptr<label> l = std::make_unique<label>(data_source, "description"_uri);
    l->word_wrap(true);
    c->add_widget("description"_uri, std::move(l));
    return c;
}

lue::data::data_source_memory create_demo_1()
{
    using namespace lue::data;
    data_source_memory ds{"/"_uri};
    ds.set("title", "Harry Potter and the Philosopher's Stone"s);
    ds.set("description", "Orphaned as a baby, Harry Potter is entrusted to his only living relatives, the Dursley family that wasn't related to the wizardry world, by Professor Albus Dumbledore, Professor Minerva McGonagall, and key keeper Rubeus Hagrid from Hogwarts School of Witchcraft and Wizardry."s);
    ds.set("stars", 4);
    return ds;
}