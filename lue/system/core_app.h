#pragma once


namespace lue::system {


class core_app {
public:
    struct cleanup
    {
        explicit cleanup(core_app& instance);
        cleanup(const cleanup&) = delete;
        cleanup& operator=(const cleanup&) = delete;
        cleanup(cleanup&&);
        cleanup& operator=(cleanup&&) = delete;
        ~cleanup();

    private:
        core_app&   app_instance_ref_;
    };


    core_app();
    core_app(int& argc, char* argv[]);
    core_app(const core_app&) = delete;
    core_app& operator=(const core_app&) = delete;
    core_app(core_app&&);
    core_app& operator=(core_app&&) = delete;

    [[nodiscard("NEVER discard app::cleanup")]] cleanup initialize(int& argc, char* argv[]) { do_initialize(argc, argv); return cleanup{*this}; }
    void run() { do_run(); }


    virtual ~core_app();


    void                exit_code_set   (int value)     { exit_code_ = value;   }
    int                 exit_code       () const        { return exit_code_;    }

protected:

    virtual void        do_initialize   (int& argc, char* argv[]) = 0;
    virtual void        do_run          () = 0;
    virtual void        do_cleanup      () = 0;

private:
    int                 exit_code_  {0};

};

} // namespace lue::system
