# NOTE
I have no intentions of mantaining this software.

**Pull requests will be ignored.**

# Another NOTE

I accidentally forked the project when checking it out on Github, but whatever.
Maybe I'll start developing on this project and add functionality I'd like.

Vegard.

**Pull requests might still be ignored.**

## What is it?
SCC (SDL C++ Classes) is a small header-only C++11 library that provides
classes for nearly all SDL types, including those coming from some subprojects
(namely: SDL_image, SDL_ttf and SDL_mixer).  
SCC is NOT:
- A game engine
- A library that provides colliders or physics simulation
- An additional layer of abstraction on top of SDL, or an attempt to "hide"
SDL from users
- A library that provides any functionality that isn't in SDL or subprojects

## Why?
SDL was written in C, which allow users to use it natively with C++. So why this?  
Most C++ SDL users will want to write classes around the SDL types, and they'll
probably want said classes to conform to the C++ memory management philosophy
(acquiring resources in the constructor and releasing them in the destructor).
Also, since we're using C++11, resources are always allocated with use of smart
pointers, which makes memory leaks much less likely.  
In short, SCC intends to expose the SDL API in the C++ way.

## Documentation
Please see the comments left in the header files, and the test units for
usage examples of the respective functionality they're testing.
A great deal of classes and functions have similar names to their SDL
countertypes, so most of it should be straightforward.

## Requirements
- SDL 2.0 or greater
- A C++11 compiler (known to compile with g++ 5.4 and 6.3)
- (Should work on) Any system supported by SDL (only tested under Linux)

## Not Done
See notDone.txt for things I intended to do, but didn't.

## Misc notes
- I haven't implemented an EventManager class or anything like that, not only
  because it doesn't exist in SDL, but also it seems to me that polling events
  the old way is good enough. An Event class wouldn't make sense either, since
  the SDL_Event's are owned by SDL's event queue
  (you don't free or allocate them yourself)
