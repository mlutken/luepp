#pragma once

#include "system/app.h"

namespace lue::gui::common {

class app : public system::app
{
public:
    app() = delete;
    app(int& argc, char *argv[]);

};

} // namespace lue::gui::common
