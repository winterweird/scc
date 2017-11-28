*** What is it? ***
SCC (SDL C++ Classes) is a small C++11 library that provides classes for several
SDL types, including types and functionality coming from some subprojects
(namely: SDL_image, SDL_ttf and SDL_mixer).

SCC is NOT:
	- A game engine
	- A library that provides colliders or physics simulation
	- An additional layer of abstraction on top of SDL, or an attempt
	  to "hide" SDL from users
	- A library that provides any functionality that isn't in SDL or
	  subprojects

*** Why? ***
SDL was written in C, which allow users to use it natively with C++ without
this library. So why is it here?
Most C++ SDL users will want to write classes around the SDL types, and they'll
probably want said classes to conform to the C++ memory management philosophy
(acquiring resources in the constructor and releasing them in the destructor).
Also, since we're using C++11, resources are always allocated with use of smart
pointers, which makes memory leaks much less likely.
In short, SCC intends to expose the SDL API in the C++ way.

*** Documentation ***
There is no documentation (yet?), but please see the comments left in the header
files. Also, a great deal of classes and
functions have similar names to their SDL countertypes, so most of it should be
straightforward.

*** Requirements ***
- SDL 2.0 or greater
- A C++11 compiler (known to compile with g++ 5.4)
- (Should work on) Any system supported by SDL (only tested under Linux so far)

*** To Do ****
SCC doesn't yet cover all of SDL enormous functionality: see the todo file.

*** Misc. notes ***
- You can disable the SDL subprojects you don't want to use, at compile time;
  see config.hpp.
- I haven't implemented an EventManager class or anything like that, not only
  because it doesn't exist in SDL, but also it seems to me that polling events
  the old way is good enough. An Event class wouldn't make sense either, since
  the SDL_Event's are owned by SDL's event queue (you don't free or allocate
  them yourself)
