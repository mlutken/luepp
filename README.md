A fast STL inspired C++ library for embedded, games and realtime
================================================================
Fast standard library like classes in static versions along with lockless fifos etc.

*note: * This library is replacing the old named "nestle". I you still need the C parts of the 
original library you can find it here: https://github.com/mlutken/nestle


Motivation
----------
This library aims to provide a way for mainly embedded and fast realtime software
projects to use the functionality from the Standard Template Library.

The problem commonly faced on embedded platforms is that you can often only
use the "Free standing" C++ implementation and thus you will run into problems
when trying to use the compiler supplied STL implementation.


*Performance and multithreading*
A secondary goal with this library is to provide classes (also in C) for performance critical
projects like games where you often want to avoid dynamic allocation and need to use effective
lock free structures like for example queues.


Current features C++
--------------------
Support for C++11 and newer for most features. Some things like move semantics and the
coming non dynamically allocation estl::function will need C++11 or newer.

- *vector_s*: A statically allocated vector that otrherwise behaves as your friendly std::vector.
  So it's _not_ the same as std::array!
- *map_s*: A statically allocated map. It works by using an internal vector_s, which is then sorted.
- *atomic*: Simple atomic for integer types only.
- *mutex*: Simple mutex and lock_guard.
- *srsw_fifo*: Single reader, single writer lockless fifo.
- *mrsw_fifo*: _(planned) Multiple reader, single writer lockless fifo._
- *srmw_fifo*: _(planned) Single reader, multiple writer lockless fifo._
- *mrmw_fifo*: _(planned) Multiple reader, multiple writer lockless fifo._
- *shared_ptr*: Simple shared_ptr and weak_ptr implementation.
- *unique_ptr*: _(planned)Simple unique_ptr. C++11 and newer only!_
- *state_machine*: A novel approach to state machines. Emphasis on splitting state changes from actions.
   Easily create unit tests that tests all transitions instantly. Can draw Graphviz "dot" state machie diagrams
   directly from the C++ code!



CMake
-----
For anyone looking for a good "template" for a new project that uses CMake as build
system I believe you will find this project usefull.
I have tried to the best of my effort to use the most recent best practices in regards
to CMake.

# Using in existing non CMake projects
The code is written using CMake a little as possible for platfor specific stuff, leaving most
of this in plain header files using the C/C++ preprocessor. Thus it's relatively simple to simply add
all the .c and .cpp files into your own build system and set a few preprocessor macros on your compiler
command line. These are the relevant ones. (see more in the root CMakeLists.txt file,
tools/cmake/default_versions.cmake, config/lueppdefault_config.h ).

- -DCXX_STANDARD_LIBRARY=(Possible values: 11, 14, 17, 20)
- -DCXX_STANDARD=(Possible values: 11, 14, 17, 20)
- -DLUEPP_PLATFORM_TYPE=(Possible values: 1 (LUEPP_PLATFORM_TYPE_DESKTOP), 2 (LUEPP_PLATFORM_TYPE_EMBEDDED))
- -DLUEPP_SYSTEM_NAME=(Possible values: 1 (LUEPP_SYSTEM_NAME_LINUX), 2 (LUEPP_SYSTEM_NAME_WINDOWS), 3 (LUEPP_SYSTEM_NAME_OSX), 4 (LUEPP_SYSTEM_NAME_FREERTOS) )

*Some CMake "features" worth mentioning:*
- Multiple platform support, using toolchain files for cross compiling
- Libraries use the CMake target_xx commands which makes sure that dependencies, includes, linker paths etc.
  are "inherited" by other libraries or executables that "depend" on them.
- Unit test subdirectories called "unittest" in each subfolder, which uses gtest. It's very easy to add a new unit test file in these
  subdirectories.
- Playground subdirectories called "playground", where you can easily add simple executables usefull for any adhoc testing.


*Lastly:* I highly recommend using this project from within the freely available QTCreator.
It's really fast and capable IDE for C and C++ code - also when you don't use any QT classes at all.

Known limitations
-----------------
- Windows support has not been tested at all yet. Should not be complicated to add though.
- Mac support has not been tested at all yet. Should not be complicated to add though.


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
It's an ambitious project and if you are willing to help please drop me a message.


