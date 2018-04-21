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

#ifndef SCC_TEXTURE_HPP
#define SCC_TEXTURE_HPP

#include <memory>
#include <stdexcept>
#include "null.hpp"
#include "cstylealloc.hpp"
#include "rwops.hpp"
#include "renderer.hpp"
#include "surface.hpp"

#ifdef HAVE_SDL_TTF
# include "truetypefont.hpp"
#endif

namespace SDL {

class RWops;
class TrueTypeFont;
class Surface;
class Renderer;

class Texture {
	friend class Renderer;
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
	int query(Uint32 *format, int *access, int *w, int *h) const
	{
		return SDL_QueryTexture(texture_.get(), format, access, w, h);
	}

	// convenience wrappers around query(); they return by value instead.
	// Errors are ignored.
	int getWidth() const;
	int getHeight() const;

	// warning:
	// as SDL documentation states, the data in pixels is not necessarily
	// meaningful; you're supposed to write to it, not to read from it.
	bool lock(const SDL_Rect *rect, void **pixels, int *pitch)
	{
		return SDL_LockTexture(texture_.get(), rect, pixels, pitch) >= 0;
	}
	void unlock() { SDL_UnlockTexture(texture_.get()); }

	bool setColorMod(Uint8 r, Uint8 g, Uint8 b)
	{
		return SDL_SetTextureColorMod(texture_.get(), r, g, b) >= 0;
	}
	bool getColorMod(Uint8 *r, Uint8 *g, Uint8 *b)
	{
		return SDL_GetTextureColorMod(texture_.get(), r, g, b) >= 0;
	}

	bool setAlphaMod(Uint8 alpha)
	{
		return SDL_SetTextureAlphaMod(texture_.get(), alpha) >= 0;
	}
	bool getAlphaMod(Uint8 *alpha)
	{
		return SDL_GetTextureAlphaMod(texture_.get(), alpha) >= 0;
	}

	bool setBlendMode(SDL_BlendMode blendMode)
	{
		return SDL_SetTextureBlendMode(texture_.get(), blendMode) >= 0;
	}
	bool getBlendMode(SDL_BlendMode *blendMode)
	{
		return SDL_GetTextureBlendMode(texture_.get(), blendMode) >= 0;
	}

	// "You need a renderer to create a SDL_Texture, therefore you can only
	// use this function with an implicit OpenGL context from
	// SDL_CreateRenderer(), not with your own OpenGL context.
	// If you need control over your OpenGL context, you need to write your
	// own texture-loading methods."
	// <wiki.libsdl.org/SDL_GL_BindTexture>
	//
	bool bind(float *w = NULL, float *h = NULL)
	{
		return SDL_GL_BindTexture(texture_.get(), w, h);
	}
	bool unbind()
	{
		return SDL_GL_UnbindTexture(texture_.get());
	}

	Texture(const Texture &that) = delete;
	Texture(Texture &&that) = default;
	~Texture() = default;
	Texture & operator=(Texture that) { swap(*this, that); return *this; }
	friend void swap(Texture &first, Texture &second) noexcept
	{
		using std::swap;
		swap(first.texture_, second.texture_);
	}

	struct Deleter {
		void operator()(SDL_Texture *texture)
		{
			SDL_DestroyTexture(texture);
		}
	};
private:
	std::unique_ptr<SDL_Texture, Deleter> texture_;
};

Texture::Texture(SDL_Renderer *renderer, Uint32 format, int access,
	int width, int height)
	: texture_{CStyleAlloc<Texture::Deleter>::alloc(SDL_CreateTexture,
		"Making texture failed",
		renderer, format, access, width, height)}
{}

Texture::Texture(SDL_Renderer *renderer, const Surface &surface)
	: texture_{CStyleAlloc<Texture::Deleter>::alloc(
		SDL_CreateTextureFromSurface,
		"Making texture from surface failed",
		renderer, surface.surface_.get())}
{}

#ifdef HAVE_SDL_IMAGE
Texture::Texture(SDL_Renderer *renderer, const char *imagePath)
	: Texture(renderer, RWops(imagePath, "rb"))
{}

Texture::Texture(SDL_Renderer *renderer, const RWops &image)
	: texture_{FromRWops<Texture::Deleter>::load(image,
		[](SDL_RWops *rwops, int freesrc, SDL_Renderer *renderer)
		-> SDL_Texture*
		{
			return IMG_LoadTexture_RW(renderer, rwops, freesrc);
		}
		, "Making texture from image failed", renderer)}
{}
#endif

#ifdef HAVE_SDL_TTF
// unfortunately, SDL_ttf has no function to load to a texture directly
Texture::Texture(SDL_Renderer *renderer, const char *text, TrueTypeFont &font,
	SDL_Color color) : Texture(renderer, Surface(text, font, color))
{}
#endif

int Texture::getWidth() const
{
	int width;
	SDL_QueryTexture(texture_.get(), NULL, NULL, &width, NULL);
	return width;
}

int Texture::getHeight() const
{
	int height;
	SDL_QueryTexture(texture_.get(), NULL, NULL, NULL, &height);
	return height;
}

} // namespace SDL

#endif
