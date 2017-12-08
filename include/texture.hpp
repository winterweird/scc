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

#ifndef SCC_TEXTURE_HPP
#define SCC_TEXTURE_HPP

#include <memory>
#include <SDL.h>
#include "config.hpp"

namespace SDL {

class RWops;
class TrueTypeFont;
class Surface;
class Renderer;

class Texture {
	friend class Renderer;

	// TODO texture streaming
public:
	// The ctors are meant to be called from Renderer's makeTexture().

	// empty texture. You probably want access to be
	// SDL_TEXTUREACCESS_STREAMING or SDL_TEXTUREACCESS_TARGET
	Texture(SDL_Renderer *renderer, Uint32 format, int access,
		int width, int height);

	Texture(SDL_Renderer *renderer, const Surface &surface);

#ifdef HAVE_SDL_IMAGE
	// non-bitmap images. For bitmaps, make a Surface from them first
	Texture(SDL_Renderer *renderer, const char *imagePath);
	Texture(SDL_Renderer *renderer, const RWops &image);
#endif

#ifdef HAVE_SDL_TTF
	Texture(SDL_Renderer *renderer, const char *text, TrueTypeFont &font,
		SDL_Color color);
#endif

	// Wrapper around SDL_QueryTexture(). You can pass NULL for parameters
	// you're not interested in.
	int query(Uint32 *format, int *access, int *width, int *height) const;

	// convenience wrappers around query(); they return by value instead.
	// Errors are ignored.
	int getWidth() const;
	int getHeight() const;

	Texture(const Texture &that) = delete;
	Texture(Texture &&that) = default;
	~Texture() = default;
	Texture & operator=(Texture that);
	friend void swap(Texture &first, Texture &second) noexcept;

	struct Deleter {
		void operator()(SDL_Texture *texture)
		{
			SDL_DestroyTexture(texture);
		}
	};
private:
	std::unique_ptr<SDL_Texture, Deleter> texture_;
};

} // namespace SDL

#endif
