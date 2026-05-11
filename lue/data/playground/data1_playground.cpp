#include <iostream>
#include "data/data_source.h"


using namespace std;
using namespace lue::data;

void data_source_playground_1();


int main()
{
    cerr << "--- data1 playground ---\n";

    data_source_playground_1();
    return 0; 
}


void data_source_playground_1()
{
    cerr << " --- data_source_playground_1() ---\n";
    data_value_vec ds_vec {1, 2, 3};
    auto ds_leaf = make_shared<data_source>("/leaf");
    ds_leaf->set("title", "Cpp Weekly - ep 500"s);
    ds_leaf->set("stars", 3);
    ds_leaf->set("vec", ds_vec);
    
    // ds_leaf->dbg_print();
    
    data_source ds_root{"/"};
    ds_root.set("title", "The Trump Report - ep 345"s);
    ds_root.set("stars", 4);
    ds_root.set("ai_generated", false);
    ds_root.set("pos", make_shared<my_point>(2, 3));
    ds_root.set("/leaf", ds_leaf);
    
    ds_root.dbg_print();
}
