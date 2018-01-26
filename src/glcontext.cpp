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

#include <memory>
#include <SDL.h>
#include "config.hpp"
#include "glcontext.hpp"

using SDL::GLContext;

GLContext::GLContext() : context_(NULL) {}

GLContext::GLContext(SDL_Window *window) :
	context_{SDL_GL_CreateContext(window)}
{}

GLContext::GLContext(GLContext &&that) : GLContext()
{
	// swap the newly-constructed empty object with the rvalue recieved
	swap(*this, that);
}

GLContext::~GLContext()
{
	if(context_ != NULL) {
		SDL_GL_DeleteContext(context_);
	}
}
