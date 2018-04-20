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
#include <iomanip>
#include <string>
#include <cstring>
#include <SDL.h>
#include "rwops.hpp"

const int ERR_SDL_INIT = -1;
const int BUFFER_SIZE = 32;

// 256 bytes from /dev/urandom. Conveniently, none turned out to be 0xff
unsigned char mem[] = {
	0x1c, 0x83, 0xc1, 0xba, 0xdd, 0x32, 0x75, 0x25, 0x8f, 0x1f, 0xc5, 0xa7,
	0x09, 0xa1, 0x5c, 0x49, 0x07, 0x8d, 0xc2, 0xd0, 0xae, 0x89, 0x69, 0x66,
	0xfb, 0xa0, 0x56, 0x03, 0x94, 0x4f, 0x7e, 0x0f, 0x37, 0x17, 0x4c, 0x5c,
	0xf4, 0x37, 0x63, 0x47, 0x9c, 0xbd, 0xf5, 0xfe, 0x97, 0x86, 0xc1, 0x20,
	0x63, 0xaa, 0x40, 0x79, 0xf3, 0x77, 0xe2, 0x46, 0x52, 0x68, 0x2d, 0xd2,
	0x79, 0x85, 0xd8, 0x03, 0x93, 0x88, 0x39, 0xf5, 0xea, 0x74, 0x7e, 0xdb,
	0x95, 0x3c, 0x03, 0xf0, 0x8f, 0x1c, 0x1f, 0x1d, 0x37, 0xdc, 0x17, 0xbb,
	0x37, 0xcf, 0x9c, 0xb2, 0x8b, 0x25, 0xb3, 0xed, 0xe5, 0x5a, 0x0f, 0x6f,
	0x2d, 0x1d, 0xb7, 0x64, 0x16, 0x78, 0xcf, 0x34, 0x27, 0xb2, 0x2c, 0xce,
	0x26, 0xf2, 0x3c, 0xdf, 0xd5, 0xd7, 0x7f, 0x96, 0xed, 0x78, 0xec, 0xc2,
	0xa0, 0xf2, 0x6c, 0xfd, 0xb0, 0x24, 0x55, 0x11, 0xcd, 0x81, 0x4a, 0x73,
	0xad, 0x37, 0x31, 0xa5, 0x43, 0xfb, 0xc5, 0x24, 0xbe, 0x80, 0x1c, 0xe0,
	0xe9, 0x77, 0x83, 0x71, 0xe7, 0x4c, 0xc5, 0x91, 0x5c, 0xf6, 0xd2, 0x71,
	0x19, 0xcb, 0x40, 0xf0, 0x49, 0x91, 0x0e, 0x24, 0x6c, 0x7c, 0x5d, 0x1d,
	0x78, 0x77, 0x94, 0x4f, 0x1d, 0x6f, 0xfe, 0x4d, 0xe3, 0x4a, 0x95, 0xad,
	0x28, 0x11, 0xd7, 0x9e, 0x48, 0x7f, 0x47, 0xdd, 0xde, 0x9f, 0x36, 0x04,
	0xf5, 0x2f, 0xd4, 0xc4, 0x2b, 0x15, 0x5c, 0x86, 0xab, 0x4c, 0x5f, 0xa1,
	0x5c, 0xfc, 0x0c, 0x34, 0xdb, 0x9d, 0x06, 0x95, 0x86, 0xba, 0xb9, 0x2b,
	0x84, 0xf2, 0x8f, 0xd8, 0xf3, 0x89, 0x01, 0x69, 0xb9, 0x17, 0xf2, 0x4c,
	0xae, 0x3d, 0xf6, 0xc3, 0xf6, 0x3c, 0x72, 0x82, 0x28, 0xfc, 0xd7, 0x79,
	0x17, 0xa4, 0x41, 0x64, 0xc9, 0xae, 0x21, 0xbb, 0x22, 0x87, 0xe0, 0x6a,
	0x7d, 0xa0, 0x29, 0x4d
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

// returns -1 if c isn't found; otherwise, returns the offset in bytes
// between the beginning of the memory and the byte
Sint64 findAndMarkByte(SDL::RWops &memory, int c)
{
	char buffer[BUFFER_SIZE];
	Sint64 offset = -1;
	size_t bytesRead;

	do {
		Sint64 memLastPos = memory.tell();
		bytesRead = memory.read(buffer, sizeof(char), BUFFER_SIZE);

		// we can't use strchr() because buffer isn't null terminated
		char *byteFirstMatch = static_cast<char*>(memchr(buffer, c,
			BUFFER_SIZE));
		if(byteFirstMatch != NULL) {
			offset = memLastPos + (byteFirstMatch - buffer);
			*byteFirstMatch = 0xff; // "mark" char

			// go back and write buffer, with the marked char
			memory.seek(memLastPos, RW_SEEK_SET);
			memory.write(buffer, sizeof(char), BUFFER_SIZE);
			break;
		}
	} while(bytesRead == BUFFER_SIZE);

	memory.seek(0, RW_SEEK_SET); // rewind
	return offset;
}

// except for 0xff, that's not valid
int askForByte()
{
	int num;
	bool numIsValid = false;
	while(!numIsValid) {
		std::string strTyped;

		std::cout << "Type a number between 0x00 and 0xfe: ";
		std::cin >> strTyped;
		try {
			num = std::stoi(strTyped, nullptr, 0); // guess base
		} catch(const std::invalid_argument& e) {
			std::cout << "Format not recognized. Try again."
				<< std::endl;
			continue;
		}
		if(num < 0x00 || num > 0xfe) {
			std::cout << "Number is not in the requested interval."
				<< std::endl;
			continue;
		}
		numIsValid = true;
	}
	return num;
}

bool shouldItQuit() {
	bool quit = true;
	bool validAnswer = false;
	while(!validAnswer) {
		std::string answer;
		std::cout << "continue? (y/n) ";
		std::cin >> answer;
		if(answer == "y") {
			quit = false;
			validAnswer = true;
		} else if(answer == "n") {
			quit = true;
			validAnswer = true;
		}
	}
	return quit;
}

void test()
{
	SDL::RWops memory(mem, sizeof(mem));
	bool quit = true;
	do {
		int byteChosen = askForByte();
		Sint64 offset = findAndMarkByte(memory, byteChosen);
		if(offset >= 0) {
			std::cout << "Byte " << std::setbase(16) << byteChosen
				<< std::setbase(10) << " found at offset "
				<< offset << std::endl;
		} else {
			std::cout << "Byte " << std::setbase(16) << byteChosen
				<< std::setbase(10) << " not found."
				<< std::endl;
		}
		quit = shouldItQuit();
	} while(!quit);
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
