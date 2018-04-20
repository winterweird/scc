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

#ifndef SCC_SURFACE_HPP
#define SCC_SURFACE_HPP

#include <memory>
#include "null.hpp"
#include "cstylealloc.hpp"
#include "rwops.hpp"

#ifdef HAVE_SDL_TTF
# include "truetypefont.hpp"
#endif

namespace SDL {

class RWops;
class Texture;
#ifdef HAVE_SDL_TTF
class TrueTypeFont;
#endif

class Surface {
	// for converting Surface into Texture
	friend class Texture;
public:
	static Surface fromBitmap(const char *path)
	{
		return Surface(RWops(path, "rb"), FromBitmap::dummy);
	}
	static Surface fromBitmap(const RWops &bitmap)
	{
		return Surface(bitmap, FromBitmap::dummy);
	}

#ifdef HAVE_SDL_IMAGE
	static Surface fromImage(const char *path)
	{
		return Surface(RWops(path, "rb"), FromImage::dummy);
	}
	static Surface fromImage(const RWops &image)
	{
		return Surface(image, FromImage::dummy);
	}
#endif

#ifdef HAVE_SDL_TTF
	static Surface fromText(const char *text, TrueTypeFont &font,
		SDL_Color color)
	{
		return Surface(text, font, color);
	}
#endif
	friend bool blit(Surface &src, Surface &dest,
		const SDL_Rect *srcRect = NULL, SDL_Rect *destRect = NULL)
	{
		return SDL_BlitSurface(src.surface_.get(), srcRect,
			dest.surface_.get(), destRect) >= 0;
	}
	friend bool blitScaled(Surface &src, Surface &dest,
		const SDL_Rect *srcRect = NULL, SDL_Rect *destRect = NULL)
	{
		return SDL_BlitScaled(src.surface_.get(), srcRect,
			dest.surface_.get(), destRect) >= 0;
	}

	// these overloads are for blitting to the Window's surface
	friend bool blit(Surface &src, SDL_Surface *dest,
		const SDL_Rect *srcRect = NULL, SDL_Rect *destRect = NULL)
	{
		return SDL_BlitSurface(src.surface_.get(), srcRect,
			dest, destRect) >= 0;
	}
	friend bool blit(SDL_Surface *src, Surface &dest,
		const SDL_Rect *srcRect = NULL, SDL_Rect *destRect = NULL)
	{
		return SDL_BlitSurface(src, srcRect,
			dest.surface_.get(), destRect) >= 0;
	}
	friend bool blitScaled(Surface &src, SDL_Surface *dest,
		const SDL_Rect *srcRect = NULL, SDL_Rect *destRect = NULL)
	{
		return SDL_BlitScaled(src.surface_.get(), srcRect,
			dest, destRect) >= 0;
	}
	friend bool blitScaled(SDL_Surface *src, Surface &dest,
		const SDL_Rect *srcRect = NULL, SDL_Rect *destRect = NULL)
	{
		return SDL_BlitScaled(src, srcRect,
			dest.surface_.get(), destRect) >= 0;
	}

	int getWidth() const { return surface_->w; }
	int getHeight() const { return surface_->h; }
	int getPitch() const { return surface_->pitch; }
	void *getPixels() const { return surface_->pixels; }
	Uint32 getPixelFormat() const { return surface_->format->format; }

	Surface(const Surface &that) = delete;
	Surface(Surface &&that) = default;
	~Surface() = default;
	Surface & operator=(Surface that) { swap(*this, that); return *this; }
	friend void swap(Surface &first, Surface &second) noexcept
	{
		using std::swap;
		swap(first.surface_, second.surface_);
	}

	struct Deleter {
		void operator()(SDL_Surface *surface)
		{
			SDL_FreeSurface(surface);
		}
	};
private:
	enum class FromBitmap { dummy };
	Surface(const RWops &bitmap, FromBitmap dummy)
		: surface_{FromRWops<Surface::Deleter>::load(bitmap,
			SDL_LoadBMP_RW, "Making surface from bitmap failed")}
	{}
#ifdef HAVE_SDL_IMAGE
	enum class FromImage { dummy };
	Surface(const RWops &image, FromImage dummy)
		: surface_{FromRWops<Surface::Deleter>::load(image,
			IMG_Load_RW, "Making surface from image failed")}
	{}
#endif
#ifdef HAVE_SDL_TTF
	Surface(const char *text, TrueTypeFont &font, SDL_Color color)
		: surface_{CStyleAlloc<Surface::Deleter>::alloc(
			TTF_RenderText_Solid,
			"Making surface from text failed", font.font_.get(),
			text, color)}
	{}
#endif
	std::unique_ptr<SDL_Surface, Deleter> surface_;
};

} // namespace SDL

#endif
