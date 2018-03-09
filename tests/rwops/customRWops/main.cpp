/*
  SDL C++ Classes
  Copyright (C) 2017 Mateus Carmo M. de F. Barbosa
 
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
#include <sstream>
#include <string>
#include <ios>
#include <SDL.h>
#include "rwops.hpp"

const int ERR_SDL_INIT = -1;

const int BUFFER_SIZE = 4;
const int CHAR_SKIP = 2;

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

Sint64 stringstreamSize(SDL_RWops *rwops)
{
	auto ss = static_cast<std::stringstream*>(rwops->hidden.unknown.data1);
	return ss == NULL ? -1 : ss->str().size();
}

Sint64 stringstreamSeek(SDL_RWops *rwops, Sint64 offset, int whence)
{
	auto ss = static_cast<std::stringstream*>(rwops->hidden.unknown.data1);
	if(ss == NULL) {
	       return -1;
	}
	std::ios_base::seekdir direction;
	switch(whence) {
	case RW_SEEK_SET:
		direction = ss->beg;
	break;
	case RW_SEEK_CUR:
		direction = ss->cur;
	break;
	case RW_SEEK_END:
		direction = ss->end;
	break;
	default:
		return -1;
	}
	// seek both input and output buffers
	ss->seekg(offset, direction);
	ss->seekp(offset, direction);

	// arbitrarily chosen over tellg(). In a real case, I don't know
	// what should be returned, but definitely not this.
	return ss->tellp();
}

size_t stringstreamRead(SDL_RWops *rwops, void *ptr, size_t size, size_t maxnum)
{
	size_t byteCount = size * maxnum;
	auto ss = static_cast<std::stringstream*>(rwops->hidden.unknown.data1);
	if(ss == NULL) {
		return 0;
	}
	ss->read(static_cast<char*>(ptr), byteCount);
	return ss->gcount();
}

size_t stringstreamWrite(SDL_RWops *rwops, const void *ptr, size_t size,
	size_t num)
{
	auto ss = static_cast<std::stringstream*>(rwops->hidden.unknown.data1);
	if(ss == NULL) {
		return 0;
	}
	size_t oldSize = ss->str().size();
	ss->write(static_cast<const char*>(ptr), size * num);
	size_t newSize = ss->str().size();
	return newSize - oldSize;
}

// NOTE: this doesn't deallocate the stringstream, only the rwops!
int stringstreamClose(SDL_RWops *rwops)
{
	if(rwops != NULL) {
		SDL_FreeRW(rwops);
	}
	return 0;
}

void test()
{
	// this works because we know ss won't outlive custom.
	std::stringstream ss;
	SDL::RWops custom(stringstreamSize, stringstreamSeek, stringstreamRead,
		stringstreamWrite, stringstreamClose, SDL_RWOPS_UNKNOWN, &ss);

	std::string str = "abcdefghijklmnopqrstuvwxyz";
	char buffer[BUFFER_SIZE];
	custom.write(str.c_str(), sizeof(char), str.size());
	std::cout << "output buffer position after writing: " << custom.tell()
		<< std::endl;

	custom.seek(0, RW_SEEK_SET);
	custom.read(buffer, sizeof(char), BUFFER_SIZE);
	std::cout << BUFFER_SIZE << " first characters: ";
	std::cout.write(buffer, BUFFER_SIZE); // buffer isn't null terminated
	std::cout << std::endl;

	custom.seek(CHAR_SKIP, RW_SEEK_CUR);
	custom.read(buffer, sizeof(char), BUFFER_SIZE);
	std::cout << BUFFER_SIZE << " characters after skipping " << CHAR_SKIP
		<< " characters from the previous position: ";
	std::cout.write(buffer, BUFFER_SIZE);
	std::cout << std::endl;

	custom.seek(-BUFFER_SIZE, RW_SEEK_END);
	custom.read(buffer, sizeof(char), BUFFER_SIZE);
	std::cout << BUFFER_SIZE << " last characters: ";
	std::cout.write(buffer, BUFFER_SIZE);
	std::cout << std::endl;

	custom.seek(0, RW_SEEK_SET); // just in case
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
