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

    // void push (std::unique_ptr<holder_base> )
    // {
    //     auto holder_ptr = std::unique_ptr<holder_base>(new holder(std::move(function)));
    //     queue_[size_++] = cmd_t{std::move(holder_ptr)};
    // }

    template<class CommandCallable>
    void send (CommandCallable&& function)
    {
        auto holder_ptr = std::unique_ptr<holder_base>(new holder(std::move(function)));
        queue_[size_++] = cmd_t{std::move(holder_ptr)};
    }

    template<class CommandCallable,
             typename ... CommandArgs>
    void send (CommandCallable&& function,
              const CommandArgs&... args)
    {
        auto cmd = [=]() {
            std::invoke(function, args...);
        };
        auto holder_ptr = std::unique_ptr<holder_base>(new holder(std::move(cmd)));
        queue_[size_++] = cmd_t{std::move(holder_ptr)};
    }

    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void send (const CommandMemberCallable& command_member_fun,
              CommandClassObject* command_class_obj_ptr,
              const CommandArgs&... args)
    {
        auto cmd = [=]() {
            std::invoke(command_member_fun, command_class_obj_ptr, args...);
        };
        queue_[size_++] = (std::move(cmd));
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
        printf("MyClass:foo [%d] -> (%d)\n", num_, p);
    }

    void bar(int p1, float p2, const std::string& p3) {
        printf("MyClass::bar [%d] -> (%d, %f, %s)\n", num_, p1, p2, p3.c_str());
    }

    int square_me(int p) const {
        auto res = p*p;
        printf("MyClas::square_me(%d) -> %d\n", p, res);
        return res;
    }

    void square_me_cb_memfun(int squared)
    {
        printf("MyClass::square_me_cb_memfun(%d)\n", squared);
    }

    int num_ = 0;
};

void bar_free_function(int p1, float p2, const std::string& p3) {
    printf("bar_free_function(%d, %f, %s)\n", p1, p2, p3.c_str());
}


void square_me_cb_free(int squared)
{
    printf("square_me_cb_free(%d)\n", squared);
}

cmd_queue q;

MyClass mc1(1);

int main() {
    printf ("--- std_fun playground ---\n");
    q.send(bar_free_function, 6, 1.6f, "Hello 6");
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

    q.send(&MyClass::square_me, &mc1, 5);
    // q.send_callback<int>(square_me_cb_free, &MyClass::square_me, &mc1, 5);
    // q.send_callback<int>(&MyClass::square_me_cb_memfun, &mc1, &MyClass::square_me, &mc1, 5);

    q.execute_all();

    return 0;
}

