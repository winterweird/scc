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

// to test, uncomment any or all of the above, and also uncomment the
// respective SCC_HAVE_FLAGS in the makefile.
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//#include <SDL_mixer.h>
#include "scc.hpp"

int main(int argc, char **argv)
{
	std::cout << "SDL version: " << SDL_MAJOR_VERSION << '.'
		<< SDL_MINOR_VERSION << '.' << SDL_PATCHLEVEL << std::endl
		<< "HAVE macros defined: " << std::endl;

#ifdef HAVE_SDL_IMAGE
	std::cout << "HAVE_SDL_IMAGE" << std::endl;
#endif

#ifdef HAVE_SDL_TTF
	std::cout << "HAVE_SDL_TTF" << std::endl;
#endif

#ifdef HAVE_SDL_MIXER
	std::cout << "HAVE_SDL_MIXER" << std::endl;
#endif

	return 0;
}
