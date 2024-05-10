#include <stdio.h>
#include <array>
#include <functional>
#include <string>

struct cmd_queue
{
    explicit cmd_queue(const char* const name) : name_(name)  {}

    using functor_t = std::function<void ()>;
    using queue_t = std::array<functor_t,32>;
private:
    template<class CommandCallable>
    void push_cmd (CommandCallable&& function)
    {
        queue_[size_++] = std::move(function);
        // printf("'%s' : push_cmd(), size: %lu\n", name_, size_);
    }

public:

    template<class CommandCallable,
             typename ... CommandArgs>
    void send (const CommandCallable& command_fun,
              const CommandArgs&... args)
    {
        auto cmd = [=]() {
            std::invoke(command_fun, args...);
        };
        push_cmd(std::move(cmd));
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
        push_cmd(std::move(cmd));
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
        push_cmd(std::move(to_send_fn));
    }

    template<typename ReturnType,
             class CallbackCallable,
             class CommandCallable,
             typename ... CommandArgs >
    void send_callback(const CallbackCallable& callback_fun,
                       std::int32_t cmd_seq_num,
                       const CommandCallable& command_fun,
                       const CommandArgs&... command_args
                       )
    {
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_fun, command_args...);
        };

        auto to_send_fn = [=](){
            auto cmd_return_value = command_fn();
            std::invoke(callback_fun, cmd_return_value, cmd_seq_num);
        };
        push_cmd(std::move(to_send_fn));
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
        push_cmd(std::move(to_send_fn));
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
        push_cmd(std::move(to_send_fn));
    }

    template<class ReturnType,
             class CallbackMemberCallable,
             class CallbackClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void send_callback(const CallbackMemberCallable& callback_member_fun,
                       CallbackClassObject* callback_class_obj_ptr,
                       std::int32_t cmd_seq_num,
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
            std::invoke(callback_member_fun, callback_class_obj_ptr, cmd_return_value, cmd_seq_num);
        };
        push_cmd(std::move(to_send_fn));
    }

    template<class ReturnType,
             class CallbackMemberCallable,
             class CallbackClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void send_response(cmd_queue* response_queue,
                       const CallbackMemberCallable& callback_member_fun,
                       CallbackClassObject* callback_class_obj_ptr,
                       std::int32_t cmd_seq_num,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                      )
    {
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
        };

        auto to_send_fn = [=]() {
            auto cmd_return_value = command_fn();
            auto response_cmd_fn = [=]() {
                std::invoke(callback_member_fun, callback_class_obj_ptr, cmd_return_value, cmd_seq_num);
            };
            response_queue->push_cmd(response_cmd_fn);
        };

        push_cmd(std::move(to_send_fn));
    }

    void execute_all() {
        for (auto i = 0u; i < size_; ++i) {
            auto& cmd = queue_[i];
            cmd();
        }
    }

    const char* const name_;
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

    void square_me_cb_memfun_seqnum(int squared, int32_t seq_num)
    {
        printf("square_me_cb_memfun_seqnum(%d, %d)\n", squared, seq_num);
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

void square_me_cb_free_seqnum(int squared, int32_t seq_num)
{
    printf("square_me_cb_free_seqnum(%d, %d)\n", squared, seq_num);
}

cmd_queue queue1("queue 1");
cmd_queue queue2("queue 2");

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
    queue1.send_callback<int>(square_me_cb_free_seqnum, 1, &MyClass::square_me, &mc1, 5);
    queue1.send_callback<int>(&MyClass::square_me_cb_memfun, &mc1, &MyClass::square_me, &mc1, 5);
    queue1.send_callback<int>(&MyClass::square_me_cb_memfun_seqnum, &mc1, 1, &MyClass::square_me, &mc1, 5);
    queue1.send_response<int>(&queue2, &MyClass::square_me_cb_memfun_seqnum, &mc1, 2, &MyClass::square_me, &mc1, 6);

    printf("--- 1 queue1.execute_all(): size: %lu ---\n", queue1.size_);
    queue1.execute_all();

    // queue2.send_callback<int>(&MyClass::square_me_cb_memfun_seqnum, &mc1, 3, &MyClass::square_me, &mc1, 7);
    printf("--- 2 queue2.execute_all(): size: %lu ---\n", queue2.size_);
    queue2.execute_all();

    return 0;
}

