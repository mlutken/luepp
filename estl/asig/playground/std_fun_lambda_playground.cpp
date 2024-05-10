#include <functional>
#include <stdio.h>
#include <array>
#include <memory>
#include <string>
#include <iostream>

struct command_base_t {
    virtual ~command_base_t() = default;
    virtual void execute() {}
    void operator()() {
        execute();
    }
};

template<class CALLABLE>
struct command_t : public command_base_t {
    explicit command_t(CALLABLE&& fn) : fn_(std::move(fn)) {}
    virtual void execute() {
        fn_();
    }
    CALLABLE fn_;
};

struct command_holder_t {
    void operator()() {
        if (holder_ptr_) {
            (*holder_ptr_)();
        }
    }
    std::unique_ptr<command_base_t> holder_ptr_ = nullptr;
};

struct cmd_queue {
    using queue_t = std::array<command_holder_t,32>;

    void push (std::unique_ptr<command_base_t> cmd)
    {
        queue_[size_++] = command_holder_t{std::move(cmd)};
    }

    template<class CommandCallable>
    void send (CommandCallable&& command_fun)
    {
        auto cmd = std::unique_ptr<command_base_t>(new command_t(std::move(command_fun)));
        queue_[size_++] = command_holder_t{std::move(cmd)};
    }

    template<class CommandCallable,
             typename ... CommandArgs>
    void send (const CommandCallable& command_fun,
              const CommandArgs&... args)
    {
        auto cmd_fn = [=]() {
            std::invoke(command_fun, args...);
        };
        auto cmd = std::unique_ptr<command_base_t>(new command_t(std::move(cmd_fn)));
        queue_[size_++] = command_holder_t{std::move(cmd)};
    }

    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void send (const CommandMemberCallable& command_member_fun,
              CommandClassObject* command_class_obj_ptr,
              const CommandArgs&... args)
    {
        // std::cerr << "FIXMENM send() command_class_obj_ptr: " << command_class_obj_ptr << "\n";
        auto cmd_fn = [=]() {
            std::invoke(command_member_fun, command_class_obj_ptr, args...);
        };
        auto cmd = std::unique_ptr<command_base_t>(new command_t(std::move(cmd_fn)));
        queue_[size_++] = command_holder_t{std::move(cmd)};
    }

    template<typename ReturnType,
             class CallbackCallable,
             class CommandCallable,
             typename ... CommandArgs >
    void send_callback(const CallbackCallable& callback_fun,
                       const CommandCallable& command_fun,
                       const CommandArgs&... command_args
                       )
    {
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_fun, command_args...);
        };

        auto to_send_fn = [=](){
            auto cmd_return_value = command_fn();
            std::invoke(callback_fun, cmd_return_value);
        };
        send(std::move(to_send_fn));
    }

    template<class ReturnType,
             class CallbackMemberCallable,
             class CallbackClassObject,
             class CommandCallable,
             typename ... CommandArgs >
    void send_callback (const CallbackMemberCallable& callback_member_fun,
                       CallbackClassObject* callback_class_obj_ptr,
                       const CommandCallable& command_fun,
                       const CommandArgs&... command_args
                       )
    {
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_fun, command_args...);
        };

        auto to_send_fn = [=](){
            auto cmd_return_value = command_fn();
            std::invoke(callback_member_fun, callback_class_obj_ptr, cmd_return_value);
        };
        send(std::move(to_send_fn));
    }

    template<class ReturnType,
             class CallbackMemberCallable,
             class CallbackClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void send_callback (const CallbackMemberCallable& callback_member_fun,
                       CallbackClassObject* callback_class_obj_ptr,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        // std::cerr << "FIXMENM send_callbak() command_class_obj_ptr: " << command_class_obj_ptr << "\n";
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
        };

        auto to_send_fn = [=](){
            auto cmd_return_value = command_fn();
            std::invoke(callback_member_fun, callback_class_obj_ptr, cmd_return_value);
        };
        send(std::move(to_send_fn));
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

cmd_queue queue1;

MyClass mc1(1);

int main() {
    printf ("--- std_fun playground ---\n");
    queue1.send(bar_free_function, 6, 1.6f, "Hello 6");
    queue1.send(&MyClass::foo, &mc1, 11);
    queue1.send(&MyClass::foo, &mc1, 21);
    queue1.send(&MyClass::foo, &mc1, 31);
    queue1.send(&MyClass::foo, &mc1, 41);
    queue1.send(&MyClass::foo, &mc1, 51);
    queue1.send(&MyClass::foo, &mc1, 61);
    queue1.send(&MyClass::foo, &mc1, 71);
    queue1.send(&MyClass::foo, &mc1, 81);
    queue1.send(&MyClass::foo, &mc1, 91);
    queue1.send(&MyClass::foo, &mc1, 101);

    queue1.send(&MyClass::bar, &mc1, 1, 1.1f, "Hello 1");
    queue1.send(&MyClass::bar, &mc1, 2, 2.2f, "Hello 2");
    queue1.send(&MyClass::bar, &mc1, 3, 1.3f, "Hello 3");
    queue1.send(&MyClass::bar, &mc1, 4, 1.4f, "Hello 4");
    queue1.send(&MyClass::bar, &mc1, 5, 1.5f, "Hello 5");

    queue1.send(&MyClass::square_me, &mc1, 5);
    queue1.send_callback<int>(square_me_cb_free, &MyClass::square_me, &mc1, 5);
    queue1.send_callback<int>(&MyClass::square_me_cb_memfun, &mc1, &MyClass::square_me, &mc1, 5);

    queue1.execute_all();

    return 0;
}

