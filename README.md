C++ library for async / multithreaded C++
==========================================
The current most "cool" part is the asynchronous signals library "lue::asig".

For example:

```signals.call_response<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, thread_a.get(), number_to_square); ```

This will trigger the `Thread_A::square_me()` to be called with the agument 12 in thread A's context. 
Then, when that function returns with the result (144) a new command back to Thread_B will be queued. 
So a little later, back in Thread_B's context the `Thread_B::callback_squared()` member function will be 
called with the result (144).



asig provides 3 main features:
 - Async Command / reponse: Call a member function in another threads context and get the response back 
 - Async timed commands: Call any function at a given time point in the receiving threads context.
 - Async publish/subscribe: Publish events, which will be sent to any entity that has subscribed.  


Other features are vector_s, which is a static allocated drop-in replacement 
for the normal std::vector. 

Lockless fifos supporting a single reader and a single writer. Lockless versions 
for multiple readers and writers are in the planning or write me if you would like to 
contribute those.

The "*_playground.cpp" files provides more examples of how to use the different parts of lue.

asig library intro
------------------
It's all about decoupling!

I the following examples we assume that we have two threads 
A and B and a global asig instance called "signals" and two threads
declared like this 
```
#include <asig/asig.h>

// Global signals and pointer to threads (This is just a demo after all)
struct Thread_A;
struct Thread_B;
lue::asig::asig signals;
std::unique_ptr<Thread_A> thread_a = nullptr;
std::unique_ptr<Thread_B> thread_b = nullptr;
```

Each thread will upon thread start need to call 
	`signals.register_receiver(class_instance_ptr);`
For every class instance for which you want to be able to asynchrounously call methods.
That will associate the class object with the given thread ID.

To elaborate on the above, we will for the examples here assume 
the two threads are defined like this:
(exluding any member functions)
```
struct Thread_A
{
	Thread_A() {
		thread_ = std::make_unique<std::thread>(&Thread_A::thread_function, this);
	}

	~Thread_A(½)  {
		is_running_ = false;
		thread_->join();
	}

	void thread_function()
	{
		signals.register_receiver(this);

		is_running_ = true;
		while(is_running_ ) {
			std::this_thread::sleep_for(1ms); 
			signals.execute_all_for_this_thread();
		}
	}

	// member functions ....

	// Member variables
	std::atomic<bool>               is_running_         {false};
	std::unique_ptr<std::thread>    thread_             {nullptr};
};
```

And now we are ready to show how to use this library to call functions, 
publish/subscribe to events and used timed calls - all asynchronously.
	

#### Calling a member function in another thread
``` signals.call(&Thread_A::set_engine_speed, thread_a.get(), 27); ```

Thats actually all. The next time thread A calls the 
```signals.execute_all_for_this_thread();```

line the `Thread_A::set_engine_speed()` member function will be called with the argument 27 in 
thread A's context.

#### Calling function in other thread and result in own thread context
From some member function in Thread_B:

```signals.call_response<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, thread_a.get(), number_to_square); ```

This will trigger the `Thread_A::square_me()` to be called with the agument 12 in thread A's context. 
Then, when that function returns with the result (144) a new command back to Thread_B will be queued. 
So a little later, back in Thread_B's context the `Thread_B::callback_squared()` member function will be 
called with the result (144).

#### Calling function in other thread and execute callback in remote thread
Given the above example. If for some reason you want to trigger the callback 
immediately in the context of Thread_A, then you simpy use the `asig::call_callback()`
like this:

```signals.call_callback<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, thread_a.get(), number_to_square); ```


#### Call an function after a timeout

```signals.timer_call_in(2s, &Thread_A::timer_expired, thread_a.get());```


#### Subscribe to an event
To subscribe to the struct my_event_t:

```subscription_ = signals.subscribe_to<my_event_t>(&Thread_B::my_event_handler, this);```

The lue::asig::subscription object can be used to unsubscribe to the event. Also it 
will unsubscribe automatically if it's destructor is called. 
This also means that you can't ignore this return value (but see subscribe_permant), 
as the subscription will be cancelled immediately if you do.

To unsubscribe "manually", simply call:

```signals.un_subscribe(subscription_);``` 

from within the thread context where you subscribed.

In some cases you might not need to ever unsbscribe for the duration of 
you application and in these case you can simply use:


```signals.subscribe_permanent<my_event_t>(&Thread_B::my_event_handler, this);```


#### Publish an event

```signals.publish_event(my_event_t{"Hello", 15});```






asig commands explained
-----------------------
Given two threads A and B and an asig class instance called signals
the 3 forms work like this:

**call():** 
From B thread:

```signals.call(&Thread_A::member_function, &thread_a, 12);```

Will simply call the given function in the receiving thread A's context.
No callback or responses are triggered

**call_callback()**
From B thread:

```signals.call_callback<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, 12);```

Will call the given function in the receiving thread A's context
and immediately call the callback function with the returned result.
So, the callback happen in the same thread (A' context) as the command itself is executed!
So in the above example the `Thread_A::square_me()` function and the callback that gets
squared number as result `Thread_B::callback_squared(int squared_val)` will happen
in thread A's context.

**call_response()**
From B thread:

```signals.call_response<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, 12);```

Will call the given function in the receiving thread A's context
and then push the response call back to thread B's context. This is the typical context at least.
In general the response will be executed in the context in which the object that holds the
`Thread_B::callback_squared()` member function exists in. Which means from the thread context
that class object has originally called `signals.register_command_receiver(this);`.
This way you get a completely asynchronous call another thread A's context,
while getting the response back in the calling thread B's context.
Remark: In case you use a lambda/std::function for the response function it will
allways use the the calling thread B's context for the response as there is no
class object registered that that we can lookup.


Current features
----------------
No dependencies other than standard C++17 or newer

- **vector_s**: A statically allocated vector that otherwise behaves as your friendly std::vector.
  So it's _not_ the same as std::array!
- **map_s**: A statically allocated map. It works by using an internal vector_s, which is then sorted.
- **srsw_fifo**: Single reader, single writer lockless fifo.
- **srsw_fifo**: Single reader, single writer lockless fifo - static version based on vector_s.
- **asig**: A very "cool" asynchronous signals library. Supports commands, publish/subscribe events and timers. 



Building
--------
# Building on Linux
$ git clone https://github.com/mlutken/lueppgit
$ mkdir -p _build/luepp$ cd _build/luepp
$ cmake ../../luepp$ make -j
...
Or Use QtCreator or any otherway you prefer when dealing with CMake projects.


Help wanted
-----------
If you are willing to help please drop me a message.


