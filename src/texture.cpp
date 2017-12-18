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
#include "rwops.hpp"

#include "renderer.hpp"
#include "surface.hpp"
#include "texture.hpp"

#ifdef HAVE_SDL_TTF
# include "truetypefont.hpp"
using SDL::TrueTypeFont;
#endif

#ifdef HAVE_SDL_IMAGE
# include <SDL_image.h>
#endif

using SDL::Renderer;
using SDL::Surface;
using SDL::Texture;

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

Texture & Texture::operator=(Texture that)
{
	swap(*this, that);
	return *this;
}

namespace SDL {

void swap(Texture &first, Texture &second) noexcept
{
	using std::swap;
	swap(first.texture_, second.texture_);
}

} // namespace SDL

int Texture::query(Uint32 *format, int *access, int *w, int *h) const
{
	return SDL_QueryTexture(texture_.get(), format, access, w, h);
}

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

bool Texture::lock(const SDL_Rect *rect, void **pixels, int *pitch)
{
	return SDL_LockTexture(texture_.get(), rect, pixels, pitch) >= 0;
}

void Texture::unlock()
{
	SDL_UnlockTexture(texture_.get());
}

bool Texture::setColorMod(Uint8 r, Uint8 g, Uint8 b)
{
	return SDL_SetTextureColorMod(texture_.get(), r, g, b) >= 0;
}

bool Texture::getColorMod(Uint8 *r, Uint8 *g, Uint8 *b)
{
	return SDL_GetTextureColorMod(texture_.get(), r, g, b) >= 0;
}

bool Texture::setAlphaMod(Uint8 alpha)
{
	return SDL_SetTextureAlphaMod(texture_.get(), alpha) >= 0;
}

bool Texture::getAlphaMod(Uint8 *alpha)
{
	return SDL_GetTextureAlphaMod(texture_.get(), alpha) >= 0;
}

bool Texture::setBlendMode(SDL_BlendMode blendMode)
{
	return SDL_SetTextureBlendMode(texture_.get(), blendMode) >= 0;
}

bool Texture::getBlendMode(SDL_BlendMode *blendMode)
{
	return SDL_GetTextureBlendMode(texture_.get(), blendMode) >= 0;
}
