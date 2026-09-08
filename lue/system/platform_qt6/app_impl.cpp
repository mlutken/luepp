#include "app_impl.h"

#include <iostream>

using namespace std;

namespace lue::system {

app_impl::app_impl(int& argc, char* argv[])
    : QApplication{argc, argv}
{
    cerr << "lue::system::app_impl::app_impl()\n";
}



} // namespace lue::system
