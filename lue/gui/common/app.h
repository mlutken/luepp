#pragma once

#include <memory>

#include <system/core_app.h>

namespace lue::gui::common {

class app_impl;

class app : public system::core_app
{
public:
    app(int& argc, char* argv[]);
    virtual ~app();

    // Disable copy and move assignment operators.
    app() = delete;
    app(const app&) = delete;
    app& operator=(const app&) = delete;
    app(app&&);
    app& operator=(app&&) = delete;

    int start();

private:
    std::unique_ptr<lue::gui::common::app_impl>     pimpl_;
};

} // namespace lue::gui::common
