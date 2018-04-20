/*
  SDL C++ Classes
  Copyright (C) 2017-2018 Mateus Carmo M. de F. Barbosa
 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
 
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
 
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <iostream>
#include <stdexcept>
#include <SDL.h>
#include "rwops.hpp"
using SDL::RWops;
using SDL::FromRWops;

const int ERR_SDL_INIT = -1;

char mem[] = {
	0, 1, 2, 3
};

bool init(Uint32 sdlInitFlags)
{
	if(SDL_Init(sdlInitFlags) < 0) {
		return false;
	}
	return true;
}

void quit()
{
	SDL_Quit();
}

void printRWopsType(SDL_RWops *rwops)
{
	std::cout << "RWops type: ";
	switch(rwops->type) {
	case SDL_RWOPS_UNKNOWN:
		std::cout << "unknown";
	break;
	case SDL_RWOPS_WINFILE:
		std::cout << "win32 file";
	break;
	case SDL_RWOPS_STDFILE:
		std::cout << "stdio file";
	break;
	case SDL_RWOPS_JNIFILE:
		std::cout << "android asset";
	break;
	case SDL_RWOPS_MEMORY:
		std::cout << "memory (read-write)";
	break;
	case SDL_RWOPS_MEMORY_RO:
		std::cout << "memory (read-only)";
	break;
	}
	std::cout << std::endl;
}

struct PseudoDeleter {
	void operator()(char *) {}
};

// has to return the same type that PseudoDeleter::operator() recieves
char * pseudoLoad(SDL_RWops *rwops, int freesrc, int extraArg)
{
	if(freesrc != 0) {
		std::cout << "error: non-zero freesrc!" << std::endl;
	}
	printRWopsType(rwops);
	std::cout << "extra argument: " << extraArg << std::endl;
	if(extraArg < 0) {
		return NULL;
	}
	return mem; // could be any non-null char*
}

void test()
{
	RWops rwops(mem, sizeof(mem));

	// in both cases, we ignore load's return value
	try {
		FromRWops<PseudoDeleter>::load(rwops, pseudoLoad,
			"error: first load throws, but it shouldn't", 42);
	} catch(const std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}

	try {
		FromRWops<PseudoDeleter>::load(rwops, pseudoLoad,
			"success: second load throws, as it should", -2);
	} catch(const std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

int main(int argc, char **argv)
{
	Uint32 sdlFlags = SDL_INIT_VIDEO;
	if(!init(sdlFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL\n");
		return ERR_SDL_INIT;
	}
	test();
	quit();
	return 0;
}
