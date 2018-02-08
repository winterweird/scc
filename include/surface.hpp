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

#ifndef SCC_SURFACE_HPP
#define SCC_SURFACE_HPP

#include <memory>
#include <SDL.h>
#include "texture.hpp"


namespace SDL {

class RWops;
#ifdef HAVE_SDL_TTF
class TrueTypeFont;
#endif

class Surface {
	// for converting Surface into Texture
	friend Texture::Texture(SDL_Renderer *renderer, const Surface &surface);
public:
	static Surface fromBitmap(const char *path);
	static Surface fromBitmap(const RWops &bitmap);

#ifdef HAVE_SDL_IMAGE
	static Surface fromImage(const char *path);
	static Surface fromImage(const RWops &image);
#endif

#ifdef HAVE_SDL_TTF
	static Surface fromText(const char *text, TrueTypeFont &font,
		SDL_Color color);
#endif

	Surface(const Surface &that) = delete;
	Surface(Surface &&that) = default;
	~Surface() = default;
	Surface & operator=(Surface that);
	friend void swap(Surface &first, Surface &second) noexcept;

	friend int blit(Surface &src, Surface &dest,
		const SDL_Rect *srcRect = NULL, SDL_Rect *destRect = NULL);

	struct Deleter {
		void operator()(SDL_Surface *surface)
		{
			SDL_FreeSurface(surface);
		}
	};
private:
	enum class FromBitmap { dummy };
	Surface(const RWops &bitmap, FromBitmap dummy);
#ifdef HAVE_SDL_IMAGE
	enum class FromImage { dummy };
	Surface(const RWops &image, FromImage dummy);
#endif
#ifdef HAVE_SDL_TTF
	Surface(const char *text, TrueTypeFont &font, SDL_Color color);
#endif
	std::unique_ptr<SDL_Surface, Deleter> surface_;
};

} // namespace SDL

#endif
