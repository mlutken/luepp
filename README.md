A modern C++ library for multithreaded and embedded C++ applications
====================================================================
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

asig library intro
------------------




Current features C++
--------------------
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


