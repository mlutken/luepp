#include <stdio.h>
#include <array>
#include <functional>
#include <string>

struct cmd_queue {
    using functor_t = std::function<void ()>;
    using queue_t = std::array<functor_t,32>;

    template<class CommandCallable>
    void send (CommandCallable&& function)
    {
        queue_[size_++] = (std::move(function));
    }

    template<class CommandCallable,
             typename ... CommandArgs>
    void send (const CommandCallable& command_fun,
              const CommandArgs&... args)
    {
        auto cmd = [=]() {
            std::invoke(command_fun, args...);
        };
        queue_[size_++] = (std::move(cmd));
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
    q.send_callback<int>(square_me_cb_free, &MyClass::square_me, &mc1, 5);
    q.send_callback<int>(&MyClass::square_me_cb_memfun, &mc1, &MyClass::square_me, &mc1, 5);

    q.execute_all();

    return 0;
}

