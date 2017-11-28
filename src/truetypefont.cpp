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
#include <stdexcept>
#include <SDL.h>
#include "config.hpp"
#include "cstylealloc.hpp"

#ifndef HAVE_SDL_TTF
# error "can't use TrueTypeFont without SDL_ttf"
#endif

#include <SDL_ttf.h>
#include "truetypefont.hpp"
using SDL::TrueTypeFont;

TrueTypeFont::TrueTypeFont(const char *path, int size)
	: font_{CStyleAlloc<TrueTypeFont::Deleter>::alloc(TTF_OpenFont,
		"Making TrueTypeFont failed", path, size)}
{}

TrueTypeFont & TrueTypeFont::operator=(TrueTypeFont that)
{
	swap(*this, that);
	return *this;
}

namespace SDL {

void swap(TrueTypeFont &first, TrueTypeFont &second) noexcept
{
	using std::swap;
	swap(first.font_, second.font_);
}

} // namespace SDL
