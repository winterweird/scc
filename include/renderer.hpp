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

#ifndef SCC_RENDERER_HPP
#define SCC_RENDERER_HPP

#include <memory>
#include <vector>
#include <stdexcept>
#include "cstylealloc.hpp"
#include "texture.hpp"

namespace SDL {

class Window;
class Surface;

// warning: it is your responsibility to make sure the Renderer doesn't outlive
// the Window that created it
class Renderer {
public:
	static const Uint32 DEFAULT_INIT_FLAGS =
		SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC
		| SDL_RENDERER_TARGETTEXTURE;

	Renderer(SDL_Window *window, Uint32 flags = DEFAULT_INIT_FLAGS);

	template <typename ... Args>
	Texture makeTexture(Args&& ... args)
	{
		return Texture(renderer_.get(), std::forward<Args>(args)...);
	}

	void present() { SDL_RenderPresent(renderer_.get()); }
	void clear() { SDL_RenderClear(renderer_.get()); }
	bool setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xff)
	{
		return SDL_SetRenderDrawColor(renderer_.get(), r, g, b, a) >= 0;
	}
	bool getDrawColor(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a) const
	{
		return SDL_GetRenderDrawColor(renderer_.get(), r, g, b, a) >= 0;
	}

	// Remember:
	// - you can only render a texture with the renderer that created it
	// - you need to call present() when you're done rendering!

	// renders a part or the entire texture with its own width and height,
	// using SDL_RenderCopy().
	// (x, y): top-left coordinates
	// src: the portion of the texture to be copied, NULL for whole texture
	bool render(Texture &texture, int x, int y,
		const SDL_Rect *src = NULL) const;

	// SDL_RenderCopy()
	bool render(Texture &texture, const SDL_Rect *src = NULL,
		const SDL_Rect *dest = NULL) const
	{
		return SDL_RenderCopy(renderer_.get(), texture.texture_.get(),
			src, dest) >= 0;
	}

	// SDL_RenderCopyEx()
	bool render(Texture &texture, const SDL_Rect *src, const SDL_Rect *dest,
		const double angle, const SDL_Point *center,
		const SDL_RendererFlip flip = SDL_FLIP_NONE) const
	{
		return SDL_RenderCopyEx(renderer_.get(), texture.texture_.get(),
			src, dest, angle, center, flip) >= 0;
	}

	bool setTarget(Texture &tex) { return setTarget(tex.texture_.get()); }
	// Does the actual work. You can use this overload to pass either NULL
	// or nullptr, which will set the target as the default
	// Note: This will call SDL_SetError if this Renderer wasn't created
	// with SDL_RENDERER_TARGETTEXTURE.
	bool setTarget(SDL_Texture *texture);

	bool setScale(float scaleX, float scaleY)
	{
		return SDL_RenderSetScale(renderer_.get(), scaleX, scaleY) >= 0;
	}
	void getScale(float *scaleX, float *scaleY) const
	{
		SDL_RenderGetScale(renderer_.get(), scaleX, scaleY);
	}

	void getViewport(SDL_Rect *rect) const
	{
		SDL_RenderGetViewport(renderer_.get(), rect);
	}
	bool setViewport(const SDL_Rect *rect)
	{
		if(rect == nullptr) { rect = NULL; }
		return SDL_RenderSetViewport(renderer_.get(), rect) >= 0;
	}

	void getLogicalSize(int *w, int *h) const
	{
		SDL_RenderGetLogicalSize(renderer_.get(), w, h);
	}
	bool setLogicalSize(int w, int h)
	{
		return SDL_RenderSetLogicalSize(renderer_.get(), w, h) >= 0;
	}

	bool getOutputSize(int *w, int *h) const
	{
		return SDL_GetRendererOutputSize(renderer_.get(), w, h) >= 0;
	}
	bool getInfo(SDL_RendererInfo *info) const
	{
		return SDL_GetRendererInfo(renderer_.get(), info) >= 0;
	}

	// "used for drawing operations (Fill and Line)" (SDL wiki)
	bool setDrawBlendMode(SDL_BlendMode mode)
	{
		return SDL_SetRenderDrawBlendMode(renderer_.get(), mode) >= 0;
	}
	bool getDrawBlendMode(SDL_BlendMode *mode) const
	{
		return SDL_GetRenderDrawBlendMode(renderer_.get(), mode) >= 0;
	}

	bool drawPoint(int x, int y) const
	{
		return SDL_RenderDrawPoint(renderer_.get(), x, y) >= 0;
	}
	bool drawPoints(const std::vector<SDL_Point> points) const
	{
		if(points.empty()) { return false; }
		return SDL_RenderDrawPoints(renderer_.get(), points.data(),
			points.size()) >= 0;
	}

	bool drawLine(int x1, int y1, int x2, int y2) const
	{
		return SDL_RenderDrawLine(renderer_.get(), x1, y1, x2, y2);
	}
	bool drawLine(const SDL_Point &p1, const SDL_Point &p2) const
	{
		return drawLine(p1.x, p1.y, p2.x, p2.y);
	}
	bool drawLines(const std::vector<SDL_Point> points) const
	{
		if(points.empty()) { return false; }
		return SDL_RenderDrawLines(renderer_.get(), points.data(),
			points.size()) >= 0;
	}

	bool drawRect(const SDL_Rect *rect) const
	{
		return SDL_RenderDrawRect(renderer_.get(), rect) >= 0;
	}
	bool drawRects(const std::vector<SDL_Rect> rects) const
	{
		if(rects.empty()) { return false; }
		return SDL_RenderDrawRects(renderer_.get(), rects.data(),
			rects.size()) >= 0;
	}

	bool fillRect(const SDL_Rect *rect) const
	{
		return SDL_RenderFillRect(renderer_.get(), rect);
	}
	bool fillRects(const std::vector<SDL_Rect> rects) const
	{
		if(rects.empty()) { return false; }
		return SDL_RenderFillRects(renderer_.get(), rects.data(),
			rects.size()) >= 0;
	}

	// TODO readPixels(), updateTexture(), setClip(), getClip(),
	// isClipEnabled()

	// gives a weak_ptr to itself.
	// The weak_ptr will expire when this object is destroyed.
	std::weak_ptr<Renderer> getWeakPtr()
	{
		return std::weak_ptr<Renderer>(thisptr_);
	}

	// renderers must NOT be copied. They belong to 1 window only.
	Renderer(const Renderer &that) = delete;
	Renderer(Renderer &&that) = default; // moving is fine though
	~Renderer() = default;
	Renderer & operator=(Renderer that) { swap(*this, that); return *this; }
	friend void swap(Renderer &first, Renderer &second) noexcept
	{
		using std::swap;
		swap(first.renderer_, second.renderer_);
	}

	struct Deleter {
		void operator()(SDL_Renderer *renderer)
		{
			SDL_DestroyRenderer(renderer);
		}
	};
private:
	std::unique_ptr<SDL_Renderer, Deleter> renderer_;
	std::shared_ptr<Renderer> thisptr_; // stores *this
};

Renderer::Renderer(SDL_Window *window, Uint32 flags)
	: renderer_{CStyleAlloc<Renderer::Deleter>::alloc(SDL_CreateRenderer,
		"Making renderer failed", window, -1, flags)}
	, thisptr_{nullptr}
{
	// empty dtor is used because we didn't allocate *this
	thisptr_ = std::shared_ptr<Renderer>(this, [](Renderer *r) {});
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
	if(texture == nullptr) { texture = NULL; }
	return SDL_SetRenderTarget(renderer_.get(), texture) >= 0;
}

} // namespace SDL

#endif
