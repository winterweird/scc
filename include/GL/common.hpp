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

#ifndef SCC_GLCOMMON_HPP
#define SCC_GLCOMMON_HPP

// common stuff needed to use OpenGL.

// FIXME the only reason this has to include SDL.h is to satisfy config.hpp;
// the SDL version check and NULL correction should have their own headers
#include <SDL.h>
#include "config.hpp"

// note: the calls to OpenGL functions done by these classes don't do any error
// checking. You're expected to either check for errors after each call, or use
// a debug callback.

#ifndef USE_GL_CLASSES
# error "USE_GL_CLASSES must be defined to use the GL helper classes"
#endif

#include <glew.h>
#include <gl.h>

#endif // SCC_GLCOMMON_HPP
