#include <iostream>
#include "data/data_source_memory.h"


using namespace std;
using namespace std::string_view_literals;
using namespace lue::data;
using namespace lue::litterals;

void data_source_playground_1();


int main()
{
    cerr << "--- data1 playground ---\n";

    data_source_playground_1();
    return 0; 
}

// ----------------
// --- my_point ---
// ----------------

struct my_point : public data_object_base
{
    my_point() = default;
    ~my_point() override = default;
    my_point(const my_point&) = default;
    my_point(my_point&&) = default;
    my_point& operator=(const my_point&) = default;
    my_point& operator=(my_point&&) = default;
    
    explicit my_point(uint32_t xval, uint32_t yval) : x(xval), y(yval) {}
    
    uint32_t x  {};
    uint32_t y  {};
    id_t            id          () const override;
    std::string     to_string   () const override;
    
};


data_object_base::id_t my_point::id() const { return __COUNTER__; }

string my_point::to_string() const
{
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}


// --------------------------------
// --- data_source_playground_1 ---
// --------------------------------

void data_source_playground_1()
{
    cerr << " --- data_source_playground_1() ---\n";
    data_value_vec ds_vec {1, 2, 3};
    auto ds_leaf = make_shared<data_source_memory>("/leaf");
    ds_leaf->set("title", "Cpp Weekly - ep 500"s);
    ds_leaf->set("stars", 3);
    ds_leaf->set("vec", ds_vec);
    
    // ds_leaf->dbg_print();
    
    data_source_memory ds_root{"/"};
    ds_root.set("title", "The Trump Report - ep 345"s);
    ds_root.set("stars", 4);
    ds_root.set("ai_generated", false);
    ds_root.set("pos", make_shared<my_point>(2, 3));
    ds_root.set("/leaf", ds_leaf);
    
    ds_root.dbg_print();
}
