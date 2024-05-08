#include <functional>
#include <stdio.h>
#include <array>
#include <memory>
#include <string>

struct holder_base {
    virtual ~holder_base() = default;
    virtual void execute() {}
    void operator()() {
        execute();
    }
};

template<class CALLABLE>
struct holder : public holder_base {
    explicit holder(CALLABLE&& fn) : fn_(std::move(fn)) {}
    virtual void execute() {
        fn_();
    }
    CALLABLE fn_;
};

struct cmd_t {
    void operator()() {
        if (holder_ptr_) {
            (*holder_ptr_)();
        }
    }
    std::unique_ptr<holder_base> holder_ptr_ = nullptr;
};

struct cmd_queue {
    using queue_t = std::array<cmd_t,32>;

    template<class CommandCallable, typename ... CommandArgs>
    void send (CommandCallable&& function, const CommandArgs&... args)
    {
        auto cmd = [=]() {
            std::invoke(function, args...);
        };
        auto holder_ptr = std::unique_ptr<holder_base>(new holder(std::move(cmd)));
        queue_[size_++] = (cmd_t{std::move(holder_ptr)});
    }

    void execute_all() {
        for (auto i = 0u; i < size_; ++i) {
            auto& cmd = queue_[i];
            cmd();
        }
    }


    queue_t queue_;
    size_t  size_ = 0;
};

struct MyClass
{
    explicit MyClass(int num) : num_(num) {}

    void foo(int p) {
        printf("Class[%d] -> (%d)\n", num_, p);
    }

    void bar(int p1, float p2, const std::string& p3) {
        printf("Class[%d] -> (%d, %f, %s)\n", num_, p1, p2, p3.c_str());
    }

    int num_ = 0;
};

cmd_queue q;

MyClass mc1(1);

int main() {
    printf ("--- std_fun_lambda playground ---\n");
    q.send(&MyClass::foo, &mc1, 11);
    q.send(&MyClass::foo, &mc1, 21);
    q.send(&MyClass::foo, &mc1, 31);
    q.send(&MyClass::foo, &mc1, 41);
    q.send(&MyClass::foo, &mc1, 51);
    q.send(&MyClass::foo, &mc1, 61);
    q.send(&MyClass::foo, &mc1, 71);
    q.send(&MyClass::foo, &mc1, 81);
    q.send(&MyClass::foo, &mc1, 91);
    q.send(&MyClass::foo, &mc1, 101);

    q.send(&MyClass::bar, &mc1, 1, 1.1f, "Hello 1");
    q.send(&MyClass::bar, &mc1, 2, 2.2f, "Hello 2");
    q.send(&MyClass::bar, &mc1, 3, 1.3f, "Hello 3");
    q.send(&MyClass::bar, &mc1, 4, 1.4f, "Hello 4");
    q.send(&MyClass::bar, &mc1, 5, 1.5f, "Hello 5");

    q.execute_all();

    return 0;
}

