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
#include <SDL.h>
#include "rwops.hpp"

const int ERR_SDL_INIT = -1;

const int BUFFER_SIZE = 64;

// This file will be read from, and won't be changed. It could be any file,
// as long as it's ASCII encoded. (Also, I obviously don't own the file)
const char *fileName = "sonnet116.txt";
// This one will be written to, and if it already existed, its previous contents
// will be discarded. You can check if it worked with vim's 'g?' command.
const char *newFileName = "sonnet116_rot13.txt";

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

void printFile(SDL::RWops &file)
{
	char buffer[BUFFER_SIZE];
	size_t charsRead = 0;
	do {
		charsRead = file.read(buffer, sizeof(char), BUFFER_SIZE - 1);
		buffer[charsRead] = '\0';
		std::cout << buffer;
	} while(charsRead == BUFFER_SIZE - 1);

	file.seek(0, RW_SEEK_SET);
}

// ASCII characters only
inline void rot13char(char &c)
{
	if((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M')) {
		c += 13;
	} else if((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z')) {
		c -= 13;
	}
}

void rot13str(char *s)
{
	for(; *s != '\0'; s++) {
		rot13char(*s);
	}
}

void rot13File(SDL::RWops &oldFile, SDL::RWops &newFile)
{
	char buffer[BUFFER_SIZE];
	size_t charsRead = 0;
	do {
		charsRead = oldFile.read(buffer, sizeof(char), BUFFER_SIZE - 1);
		buffer[charsRead] = '\0';
		rot13str(buffer);
		newFile.write(buffer, sizeof(char), charsRead);
	} while(charsRead == BUFFER_SIZE - 1);

	oldFile.seek(0, RW_SEEK_SET);
	newFile.seek(0, RW_SEEK_SET);
}

void test()
{
	SDL::RWops file(fileName, "r"); // read-only
	SDL::RWops newFile(newFileName, "w"); // write-only

	Sint64 fileSize = file.size();
	if(fileSize < 0) {
		std::cerr << "RWops::tell() failed." << std::endl;
		return;
	}
	std::cout << "File size (in bytes): " << fileSize << std::endl
		<< std::endl;

	// test 1: read
	printFile(file);

	// test 2: write
	rot13File(file, newFile);
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
