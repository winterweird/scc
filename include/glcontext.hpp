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

#ifndef SCC_GLCONTEXT_HPP
#define SCC_GLCONTEXT_HPP

#include "null.hpp"

namespace SDL {

class Window;

// like Renderer, this class shouldn't outlive the Window that created it
class GLContext {
	friend class Window; // makeCurrent()
public:
	// the constructor is called from Window's makeGLContext()
	GLContext(SDL_Window *window) : context_{SDL_GL_CreateContext(window)}
	{}

	GLContext(const GLContext &that) = delete;
	GLContext(GLContext &&that) : GLContext() { swap(*this, that); }
	~GLContext()
	{
		if(context_ != NULL) { SDL_GL_DeleteContext(context_); }
	}
	GLContext & operator=(GLContext that) { swap(*this, that); return *this; }
	friend void swap(GLContext &first, GLContext &second) noexcept
	{
		using std::swap;
		swap(first.context_, second.context_);
	}

private:
	// since this type is a typedef to void*, we can't use std::unique_ptr
	// (I think), but we don't use this knowledge in this class; instead,
	// we treat it like any non-opaque type
	SDL_GLContext context_;
	// since we're not using std::unique_ptr, it's convenient to have
	// an empty ctor to implement the move ctor.
	// I don't want users to be able to instantiate an empty object, though,
	// so this ctor has been made private.
	GLContext() : context_(NULL) {}
};

} // namespace SDL

#endif
