#include "app.h"

#include <iostream>
#include <system/platform_qt6/app_impl.h>

using namespace std;
using namespace std::string_view_literals;

namespace lue::system {

app& app::i()
{
    static app instance;
    return instance;
}

app::app()
    : core_app{}
{}

app::app(int& argc, char* argv[])
    : core_app{argc, argv}
// pimpl_{make_unique<app_impl>(argc, argv)}
{
    cerr << "lue::system::app()\n";
}


app::~app() = default;

void app::do_initialize(int& /*argc*/, char* /*argv*/[])
{
}

void app::do_run()
{
}

void app::do_cleanup() {}

} // namespace lue::system
