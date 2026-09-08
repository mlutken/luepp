#include "core_app.h"

#include <iostream>
#include <system/platform_qt6/app_impl.h>

using namespace std;
using namespace std::string_view_literals;

namespace lue::system {

core_app::core_app(int& /*argc*/, char* /*argv*/[])
{
    cerr << "lue::system::core_app()\n";
}


core_app::~core_app() = default;

// int core_app::start() {
//     return pimpl_->exec();
// }

} // namespace lue::system
