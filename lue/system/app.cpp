#include "app.h"

#include <system/platform_qt6/app_impl.h>

using namespace std;
using namespace std::string_view_literals;

namespace lue::system {

app::app() = default;

app::app(int /*argc*/, char* /*argv*/[])
// pimpl_{make_unique<app_impl>(argc, argv)}
{
}


app::~app() = default;

int app::start() {
    return pimpl_->exec();
}

} // namespace lue::system
