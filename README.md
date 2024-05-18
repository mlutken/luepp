C++ library for async / multithreaded C++
==========================================
The current most "cool" part is the asynchronous signals library "lue::asig".

It provides 3 main features:
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


asig commands explained
-----------------------
Given two threads A and B and a commands class instance called cc (command center)
the 3 forms work like this
- call()           : From B thread:
                     cc.call(&Thread_A::member_function, &thread_a, 12);
                     Will simply call the given function in the receiving thread A's context.
                     No callback or responses are triggered

- call_callback()  : From B thread:
                     cc.call_callback<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, 12);
                     Will call the given function in the receiving thread A's context
                     and immediately call the callback function with the returned result.
                     So, the callback happen in the same thread (A' context) as the command itself is executed!
                     So in the above example the Thread_A::square_me() function and the callback that gets
                     squared number as result Thread_B::callback_squared(int squared_val) will happen
                     in thread A's context.

- call_response()  : From B thread:
                     cc.call_response<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, 12);
                     Will call the given function in the receiving thread A's context
                     and then push the response call back to thread B's context. This is the typical context at least.
                     In general the response will be executed in the context in which the object that holds the
                     Thread_B::callback_squared() member function exists in. Which means from the thread context
                     that class object has originally called cc.register_command_receiver(this);.
                     This way you get a completely asynchronous call another thread A's context,
                     while getting the response back in the calling thread B's context.
                     Remark: In case you use a lambda/st::function for the response function it will
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


