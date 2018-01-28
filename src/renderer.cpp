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
#include <vector>
#include <stdexcept>
#include <SDL.h>
#include "cstylealloc.hpp"
#include "window.hpp"
#include "renderer.hpp"
using SDL::Window;
using SDL::Renderer;

Renderer::Renderer(SDL_Window *window, Uint32 flags)
	: renderer_{CStyleAlloc<Renderer::Deleter>::alloc(SDL_CreateRenderer,
		"Making renderer failed", window, -1, flags)},
	thisptr_{nullptr}
{
	// empty dtor is used because we didn't allocate this
	thisptr_ = std::shared_ptr<Renderer>(this, [](Renderer *renderer) {});
}

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

void Renderer::present()
{
	SDL_RenderPresent(renderer_.get());
}

void Renderer::clear()
{
	SDL_RenderClear(renderer_.get());
}

bool Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return SDL_SetRenderDrawColor(renderer_.get(), r, g, b, a) >= 0;
}

bool Renderer::getDrawColor(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a) const
{
	return SDL_GetRenderDrawColor(renderer_.get(), r, g, b, a) >= 0;
}

bool Renderer::render(Texture &texture, int x, int y, const SDL_Rect *src) const
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	texture.query(NULL, NULL, &dest.w, &dest.h);

	return SDL_RenderCopy(renderer_.get(), texture.texture_.get(),
		src, &dest) >= 0;
}

bool Renderer::render(Texture &texture, const SDL_Rect *src,
	const SDL_Rect *dest) const
{
	return SDL_RenderCopy(renderer_.get(), texture.texture_.get(),
		src, dest) >= 0;
}

bool Renderer::render(Texture &texture,
	const SDL_Rect *src, const SDL_Rect *dest, const double angle,
	const SDL_Point *center, const SDL_RendererFlip flip) const
{
	return SDL_RenderCopyEx(renderer_.get(), texture.texture_.get(),
		src, dest, angle, center, flip) >= 0;
}

bool Renderer::setTarget(Texture &texture)
{
	return setTarget(texture.texture_.get());
}

bool Renderer::setTarget(SDL_Texture *texture)
{
	// "Before using this function, you should check the
	// SDL_RENDERER_TARGETTEXTURE bit in the flags of SDL_RendererInfo to
	// see if render targets are supported."
	// (<wiki.libsdl.org/SDL_SetRenderTarget>)
	// Well, here it is
	SDL_RendererInfo info;
	getInfo(&info);
	if((info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
		SDL_SetError("renderer can't use texture as target");
		return false;
	}
	// so it works with nullptr too
	if(texture == nullptr) {
		texture = NULL;
	}
	return SDL_SetRenderTarget(renderer_.get(), texture) >= 0;
}

bool Renderer::setScale(float scaleX, float scaleY)
{
	return SDL_RenderSetScale(renderer_.get(), scaleX, scaleY) >= 0;
}

void Renderer::getScale(float *scaleX, float *scaleY) const
{
	SDL_RenderGetScale(renderer_.get(), scaleX, scaleY);
}

void Renderer::getViewport(SDL_Rect *rect) const
{
	SDL_RenderGetViewport(renderer_.get(), rect);
}

bool Renderer::setViewport(const SDL_Rect *rect)
{
	if(rect == nullptr) {
		rect = NULL;
	}
	return SDL_RenderSetViewport(renderer_.get(), rect) >= 0;
}

void Renderer::getLogicalSize(int *w, int *h) const
{
	SDL_RenderGetLogicalSize(renderer_.get(), w, h);
}

bool Renderer::setLogicalSize(int w, int h)
{
	return SDL_RenderSetLogicalSize(renderer_.get(), w, h) >= 0;
}

bool Renderer::getOutputSize(int *w, int *h) const
{
	return SDL_GetRendererOutputSize(renderer_.get(), w, h) >= 0;
}

bool Renderer::getInfo(SDL_RendererInfo *info) const
{
	return SDL_GetRendererInfo(renderer_.get(), info) >= 0;
}

bool Renderer::setDrawBlendMode(SDL_BlendMode mode)
{
	return SDL_SetRenderDrawBlendMode(renderer_.get(), mode) >= 0;
}

bool Renderer::getDrawBlendMode(SDL_BlendMode *mode) const
{
	return SDL_GetRenderDrawBlendMode(renderer_.get(), mode) >= 0;
}

bool Renderer::drawPoint(int x, int y) const
{
	return SDL_RenderDrawPoint(renderer_.get(), x, y) >= 0;
}

bool Renderer::drawPoints(const std::vector<SDL_Point> points) const
{
	if(points.empty()) {
		return false;
	}
	return SDL_RenderDrawPoints(renderer_.get(), points.data(),
		points.size()) >= 0;
}

bool Renderer::drawLine(int x1, int y1, int x2, int y2) const
{
	return SDL_RenderDrawLine(renderer_.get(), x1, y1, x2, y2);
}

bool Renderer::drawLines(const std::vector<SDL_Point> points) const
{
	if(points.empty()) {
		return false;
	}
	return SDL_RenderDrawLines(renderer_.get(), points.data(),
		points.size()) >= 0;
}

bool Renderer::drawRect(const SDL_Rect *rect) const
{
	return SDL_RenderDrawRect(renderer_.get(), rect) >= 0;
}

bool Renderer::drawRects(const std::vector<SDL_Rect> rects) const
{
	if(rects.empty()) {
		return false;
	}
	return SDL_RenderDrawRects(renderer_.get(), rects.data(),
		rects.size()) >= 0;
}

bool Renderer::fillRect(const SDL_Rect *rect) const
{
	return SDL_RenderFillRect(renderer_.get(), rect);
}

bool Renderer::fillRects(const std::vector<SDL_Rect> rects) const
{
	if(rects.empty()) {
		return false;
	}
	return SDL_RenderFillRects(renderer_.get(), rects.data(),
		rects.size()) >= 0;
}
