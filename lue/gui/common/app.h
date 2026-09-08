#pragma once

#include <memory>

#include <system/core_app.h>

namespace lue::gui::common {

class app_impl;

class app : public system::core_app
{
public:
    static app& i  ();

    app();

    app(int& argc, char* argv[]);
    virtual ~app();

    // Disable copy and move assignment operators.
    app(const app&) = delete;
    app& operator=(const app&) = delete;
    app(app&&);
    app& operator=(app&&) = delete;

    int start();

protected:
    void        do_initialize       (int& argc, char* argv[]) override;
    void        do_run              () override;
    void        do_cleanup          () override;


private:
    std::unique_ptr<lue::gui::common::app_impl>     pimpl_;
};

} // namespace lue::gui::common
