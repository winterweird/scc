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

#ifndef SCC_RENDERER_HPP
#define SCC_RENDERER_HPP

#include <memory>
#include <SDL.h>
#include "texture.hpp"

namespace SDL {

class Window;
class Surface;

// This class is meant to live only as a Window member object.
class Renderer {
public:
	static const Uint32 DEFAULT_INIT_FLAGS =
		SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC
		| SDL_RENDERER_TARGETTEXTURE;

	Renderer(SDL_Window *window, Uint32 flags);

	// an empty ctor is needed to construct Window. Ugly, but it works.
	Renderer();

	template <typename ... Args>
	Texture makeTexture(Args&& ... args)
	{
		return Texture(renderer_.get(), std::forward<Args>(args)...);
	}

	void renderPresent();
	void renderClear();
	void setRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xff);

	// TODO primitives drawing

	// meant to be called from Texture's render().
	void renderCopy(SDL_Texture *texture,
		const SDL_Rect *src = NULL,
		const SDL_Rect *dest = NULL) const;

	// renderers must NOT be copied. They belong to 1 window only.
	Renderer(const Renderer &that) = delete;
	Renderer(Renderer &&that) = default; // moving is fine though
	~Renderer() = default;
	Renderer & operator=(Renderer that);
	friend void swap(Renderer &first, Renderer &second) noexcept;

	struct Deleter {
		void operator()(SDL_Renderer *renderer)
		{
			SDL_DestroyRenderer(renderer);
		}
	};
private:
	std::unique_ptr<SDL_Renderer, Deleter> renderer_;
};

} // namespace SDL

#endif
