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
#include "cstylealloc.hpp"
#include "window.hpp"
#include "renderer.hpp"

using SDL::Window;
using SDL::Renderer;

Renderer::Renderer() : renderer_{nullptr}
{}

Renderer::Renderer(SDL_Window *window, Uint32 flags)
	: renderer_{CStyleAlloc<Renderer::Deleter>::alloc(SDL_CreateRenderer,
		"Making renderer failed", window, -1, flags)}
{}

Renderer & Renderer::operator=(Renderer that)
{
	swap(*this, that);
	return *this;
}

namespace SDL {

void swap(Renderer &first, Renderer &second) noexcept
{
	using std::swap;
	swap(first.renderer_, second.renderer_);
}

} // namespace SDL

void Renderer::renderPresent()
{
	SDL_RenderPresent(renderer_.get());
}

void Renderer::renderClear()
{
	SDL_RenderClear(renderer_.get());
}

void Renderer::setRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer_.get(), r, g, b, a);
}

void Renderer::renderCopy(SDL_Texture *texture, const SDL_Rect *src,
	const SDL_Rect *dest) const
{
	if(texture != NULL) {
		SDL_RenderCopy(renderer_.get(), texture, src, dest);
	} else {
		throw std::runtime_error("cannot render NULL SDL_Texture");
	}
}
