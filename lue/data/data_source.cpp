#include "data_source.h"
#include <ranges>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::string_view_literals;

template<class... Ts>
struct visit_overload_t : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
visit_overload_t(Ts...) -> visit_overload_t<Ts...>; // Deduction guide

// ----------------
// --- my_point ---
// ----------------

data_object_base::id_t my_point::id() const { return __COUNTER__; }

string my_point::to_string() const
{
    return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

// ------------------
// --- data_value ---
// ------------------

/// @see https://www.cppstories.com/2018/09/visit-variants/
string to_string(const data_value& val)
{
    return std::visit(visit_overload_t {
        [](const std::string& b)            -> string { return b;                       },
        [](bool val)                        -> string { return val ? "true" : "false";  },
        [](const data_object_base& val)     -> string { return val.to_string();         },
        [](const data_object_sp& val)       -> string { return val->to_string();        },
        [](const data_source_base& val)     -> string { return val.to_string();         },
        [](const data_source_base_sp& val)  -> string { return val->to_string();        },
        [](auto val)                        -> string { return to_string(val);          },
    }, val);
}

string to_string(const data_value_vec& vec)
{
    string s{};
    for (const auto& val: vec | views::take(vec.size()-1)) {
        s += to_string(val) + ", ";
    }
    s += to_string(vec.back());
    return s;
}

// ------------------------
// --- data_source_base ---
// ------------------------

data_source_base::data_source_base(data_path path) :
    path_{path}
{
}

void data_source_base::set(const data_path& path, data_value val)
{
    do_set(path, std::move(val));
    call_on_changed(path);
}

void data_source_base::set(const data_path& path, data_value_vec val)
{
    do_set_data_vec(path, std::move(val));
    call_on_changed(path);
}

void data_source_base::set_data_source(const data_path& path, data_source_base_sp val)
{
    do_set(path, std::move(val));
    call_on_changed(path);
}

int32_t data_source_base::as_int32(const data_path& path) const
{
    return std::get<int32_t>(as_data_value(path));
}

const string& data_source_base::as_string(const data_path& path) const
{
    return std::get<std::string>(as_data_value(path));
}

const data_value& data_source_base::as_data_value(const data_path& path) const
{
    return do_as_data_value(path);
}

void data_source_base::connect(data_changed_cb cb)
{
    data_changed_cb_vec_.push_back(std::move(cb));
}

bool data_source_base::is_read_only() const
{
    return do_is_read_only();
}

string data_source_base::to_string() const
{
    return do_to_string();
}

void data_source_base::dbg_print() const
{
    cerr << to_string() << "\n";
}

void data_source_base::call_on_changed(const data_path& path) const
{
    for (const auto& cb: data_changed_cb_vec_) {
        cb(path);
    }
}

// -------------------
// --- data_source ---
// -------------------

const data_value& data_source::do_as_data_value(const data_path& path) const
{
    static const data_value default_value{""s};
    const auto it = string_data_map_.find(path);
    if (it == string_data_map_.end()) {
        return default_value;
    }
    return it != string_data_map_.end() ? it->second : default_value;
}

void data_source::do_set(const data_path& path, data_value val)
{
    string_data_map_[path] = std::move(val);
}

void data_source::do_set_data_vec(const data_path& path, data_value_vec val)
{
    string_vec_map_[path] = std::move(val);
}

void data_source::do_set_data_source(const data_path& path, data_source_base_sp val)
{
    string_data_map_[path] = std::move(val);
}

string data_source::do_to_string() const
{
    string s;
    s += "{" + path() + "}\n";
    for (const auto& [path, val]: string_data_map_) {
        s += path + " : ";
        s += ::to_string(val) + "\n";
    }
    for (const auto& [path, vec]: string_vec_map_) {
        s += "[" + path + "]: ";
        s += ::to_string(vec) + "\n";
    }

    return s;
}

// ----------------------------------------------
// --- data_source playground and adhoc tests ---
// ----------------------------------------------

void data_source_playground_1()
{
    cerr << " --- data_source_playground_1() ---\n";
    data_value_vec ds_vec {1, 2, 3};
    auto ds_leaf = make_shared<data_source>("/leaf");
    ds_leaf->set("title", "Cpp Weekly - ep 500"s);
    ds_leaf->set("stars", 3);
    ds_leaf->set("vec", ds_vec);


    data_source ds_root{"/"};
    ds_root.set("title", "The Trump Report - ep 345"s);
    ds_root.set("stars", 4);
    ds_root.set("ai_generated", false);
    ds_root.set("pos", make_shared<my_point>(2, 3));
    ds_root.set("/leaf", ds_leaf);

    ds_root.dbg_print();



}

data_source create_demo_1()
{
    data_source ds{"/"};
    ds.set("title", "Harry Potter and the Philosopher's Stone"s);
    ds.set("description", "Orphaned as a baby, Harry Potter is entrusted to his only living relatives, the Dursley family that wasn't related to the wizardry world, by Professor Albus Dumbledore, Professor Minerva McGonagall, and key keeper Rubeus Hagrid from Hogwarts School of Witchcraft and Wizardry."s);
    ds.set("stars", 4);
    return ds;
}
