#pragma once

#include <memory>

namespace lue::system {

class app_impl;

class app {
public:
    app();
    app     (int& argc, char *argv[]);
    virtual ~app();

    int start   ();

private:
    std::unique_ptr<app_impl>     pimpl_;
};

} // namespace lue::system
