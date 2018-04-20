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

#ifndef SCC_TRUETYPEFONT_HPP
#define SCC_TRUETYPEFONT_HPP

#include <memory>
#include <stdexcept>
#include "null.hpp"
#include "cstylealloc.hpp"

#ifndef HAVE_SDL_TTF
# error "can't use TrueTypeFont without SDL_ttf"
#endif

namespace SDL {

class Surface;

class TrueTypeFont {
	friend class Surface;
public:
	TrueTypeFont(const char *path, int size);

	TrueTypeFont(const TrueTypeFont &that) = delete;
	TrueTypeFont(TrueTypeFont &&that) = default;
	~TrueTypeFont() = default;
	TrueTypeFont& operator=(TrueTypeFont that)
	{
		swap(*this, that);
		return *this;
	}
	friend void swap(TrueTypeFont &first, TrueTypeFont &second) noexcept
	{
		using std::swap;
		swap(first.font_, second.font_);
	}

	struct Deleter {
		void operator()(TTF_Font *font) { TTF_CloseFont(font); }
	};
private:
	std::unique_ptr<TTF_Font, Deleter> font_;
};

TrueTypeFont::TrueTypeFont(const char *path, int size)
	: font_{CStyleAlloc<TrueTypeFont::Deleter>::alloc(TTF_OpenFont,
		"Making TrueTypeFont failed", path, size)}
{}

} // namespace SDL

#endif
