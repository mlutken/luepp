#pragma once

#include <memory>

#include <system/core_app.h>

// TODO: Not sure we really want to use Qt signal slot etc. in a lue::core application!

namespace lue::system {

class app_impl;

class app : public core_app {
public:
    static app& i  ();

    app();


    app     (int& argc, char *argv[]);
    app(const app&) = delete;
    app& operator=(const app&) = delete;
    app(app&&);
    app& operator=(app&&) = delete;


    virtual ~app();


protected:
    void        do_initialize       (int& argc, char* argv[]) override;
    void        do_run              () override;
    void        do_cleanup          () override;

private:

    std::unique_ptr<app_impl>     pimpl_;
};

} // namespace lue::system
