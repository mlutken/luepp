#include "app_impl.h"

#include <iostream>

using namespace std;

namespace lue::gui::common {

app_impl::app_impl(int& argc, char* argv[])
    : QApplication{argc, argv}
{
    cerr << "lue::gui::common::app_impl::app_impl()\n";
}



} // namespace lue::gui::common
