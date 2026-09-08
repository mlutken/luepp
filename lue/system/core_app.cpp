#include "core_app.h"

#include <iostream>
#include <system/platform_qt6/app_impl.h>

using namespace std;
using namespace std::string_view_literals;

namespace lue::system {

core_app::core_app() {}

core_app::core_app(int& /*argc*/, char* /*argv*/[])
{
    cerr << "lue::system::core_app()\n";
}


core_app::~core_app()
{
    cerr << "core_app::DESTRUCTOR()\n";
}

core_app::cleanup::cleanup(core_app& instance)
    : app_instance_ref_{instance}
{}

core_app::cleanup::~cleanup()
{
    cerr << "core_app::cleanup::DESTRUCTOR\n";
    app_instance_ref_.do_cleanup();
}


} // namespace lue::system
