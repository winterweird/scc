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

// config.hpp: configure available libs and make preliminary checks.
// Make sure to include SDL.h before this!
//
#ifndef SCC_CONFIG_HPP
#define SCC_CONFIG_HPP

#if __cplusplus < 201103L
# error "at least C++11 is needed"
#endif

// If you've defined NULL as anything else, screw you and the NULL you rode on
#if NULL != 0
# undef NULL
# define NULL 0
#endif

#if !defined(SDL_MAJOR_VERSION) || !defined(SDL_MINOR_VERSION)
# error "Could not determine the SDL version. Is SDL.h included?"
#endif

#if SDL_MAJOR_VERSION < 2
# error "at least SDL 2.0 is needed."
#endif

// use this override to define the HAVE* macros from command line
#ifndef SCC_OVERRIDE_HAVE_MACROS
# define HAVE_SDL_IMAGE
# define HAVE_SDL_TTF
# define HAVE_SDL_MIXER
#endif

#endif // SCC_CONFIG_HPP
