#pragma once

#include <memory>

#include <system/core_app.h>

// TODO: Not sure we really want to use Qt signal slot etc. in a lue::core application!

namespace lue::system {

class app_impl;

class app : public core_app {
public:
    app     (int& argc, char *argv[]);
    app() = delete;
    app(const app&) = delete;
    app& operator=(const app&) = delete;
    app(app&&);
    app& operator=(app&&) = delete;


    virtual ~app();
    // int start   ();



private:

    std::unique_ptr<app_impl>     pimpl_;
};

} // namespace lue::system
