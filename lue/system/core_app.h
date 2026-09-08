#pragma once

// #include <memory>

namespace lue::system {

class core_app {
public:
    core_app     (int& argc, char *argv[]);
    core_app() = delete;
    core_app(const core_app&) = delete;
    core_app& operator=(const core_app&) = delete;
    core_app(core_app&&);
    core_app& operator=(core_app&&) = delete;


    virtual ~core_app();
    // int start   ();



private:
};

} // namespace lue::system
