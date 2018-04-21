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

#ifndef SCC_WINDOW_HPP
#define SCC_WINDOW_HPP

#include <memory>
#include "cstylealloc.hpp"
#include "renderer.hpp"
#include "glcontext.hpp"

namespace SDL {

// note: to actually draw something to the window, you'll have to choose between
// - creating a Renderer (2D)
// - creating a GLContext and drawing with OpenGL (3D)
// - blitting to the window's surface (no hardware acceleration)
//
class Window {
	// TODO SDL_CreateWindowFrom
public:
	static const int DEFAULT_WIDTH = 800;
	static const int DEFAULT_HEIGHT = 600;
	static const Uint32 DEFAULT_INIT_FLAGS = 0;

	// only one of these can be used at a time.
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<GLContext> context;

	// note: the argument order differs from SDL_CreateWindow() to allow
	// a better use of default values, eg Window("title", 800, 600)
	Window(const char *title,
		int width = DEFAULT_WIDTH,
		int height = DEFAULT_HEIGHT,
		int x = SDL_WINDOWPOS_UNDEFINED,
		int y = SDL_WINDOWPOS_UNDEFINED,
		Uint32 flags = DEFAULT_INIT_FLAGS);

	template <typename ... Args>
	void makeRenderer(Args&& ... args)
	{
		if(!renderer && !context) {
			renderer = std::unique_ptr<Renderer>(new Renderer(
				window_.get(), std::forward<Args>(args)...));
		}
	}

	// this not only creates an OpenGL context but also makes it the current
	// context. Will throw if the window wasn't created with the
	// SDL_WINDOW_OPENGL flag.
	template <typename ... Args>
	void makeGLContext(Args&& ... args)
	{
		if(!renderer && !context) {
			context = std::unique_ptr<GLContext>(new GLContext(
				window_.get(), std::forward<Args>(args)...));
		}
	}

	// SDL_GetWindowSize(). You may pass NULL as a parameter if you're not
	// interested in its value, though you can also use getWidth() or
	// getHeight() in that case
	void getSize(int *width, int *height) const
	{
		SDL_GetWindowSize(window_.get(), width, height);
	}

	// wrappers around SDL_GetWindowSize(), but they return the value.
	int getWidth() const;
	int getHeight() const;

	// again, you may pass NULL if you don't want a value.
	void getDrawableSize(int *width, int *height)
	{
		SDL_GL_GetDrawableSize(window_.get(), width, height);
	}

	Uint32 getID() { return SDL_GetWindowID(window_.get()); }
	Uint32 getFlags() { return SDL_GetWindowFlags(window_.get()); }

	void setTitle(const char *title)
	{
		SDL_SetWindowTitle(window_.get(), title);
	}
	const char *getTitle() { return SDL_GetWindowTitle(window_.get()); }

	void show() { SDL_ShowWindow(window_.get()); }
	void hide() { SDL_HideWindow(window_.get()); }

	void raise() { SDL_RaiseWindow(window_.get()); }

	void maximize() { SDL_MaximizeWindow(window_.get()); }
	void minimize() { SDL_MinimizeWindow(window_.get()); }
	void restore() { SDL_RestoreWindow(window_.get()); }
	bool setFullscreen(Uint32 flags)
	{
		return SDL_SetWindowFullscreen(window_.get(), flags) >= 0;
	}

	// you might want to call SDL_GL_SetSwapInterval() before using this
	void swapWindow() { SDL_GL_SwapWindow(window_.get()); }

	bool makeCurrent(GLContext &context)
	{
		return makeCurrent(context.context_);
	}
	// if you created your context through other means, you can use this
	// overload instead.
	bool makeCurrent(SDL_GLContext context)
	{
		return SDL_GL_MakeCurrent(window_.get(), context) >= 0;
	}

	// "Do not free this surface.
	// This surface will be invalidated if the window is resized.
	// After resizing a window this function must be called again to return
	// a valid surface.
	// You may not combine this with 3D or the rendering API on this window."
	// (SDL wiki)
	// Because this surface mustn't be freed, it is not and must not
	// be put in a Surface class.
	SDL_Surface *getSurface()
	{
		return SDL_GetWindowSurface(window_.get());
	}
	bool updateSurface()
	{
		return SDL_UpdateWindowSurface(window_.get()) >= 0;
	}
	bool updateSurfaceRects(const std::vector<SDL_Rect> rects)
	{
		return SDL_UpdateWindowSurfaceRects(window_.get(),
			rects.data(), rects.size()) >= 0;
	}

	bool hasRenderer() { return static_cast<bool>(renderer); }
	bool hasContext() { return static_cast<bool>(context); }

	// I don't think SDL has any way of copying windows...
	Window(const Window &that) = delete;
	Window(Window &&that) = default;
	~Window() = default;
	Window & operator=(Window that) { swap(*this, that); return *this; }
	friend void swap(Window &first, Window &second) noexcept
	{
		using std::swap;
		swap(first.window_, second.window_);
	}

	struct Deleter {
		void operator()(SDL_Window *window)
		{
			SDL_DestroyWindow(window);
		}
	};
private:
	std::unique_ptr<SDL_Window, Deleter> window_;
};

Window::Window(const char *title, int width, int height, int x, int y,
	Uint32 flags)
	: window_{CStyleAlloc<Window::Deleter>::alloc(SDL_CreateWindow,
		"Making window failed", title, x, y, width, height, flags)}
{}

int Window::getWidth() const
{
	int width;
	SDL_GetWindowSize(window_.get(), &width, NULL);
	return width;
}

int Window::getHeight() const
{
	int height;
	SDL_GetWindowSize(window_.get(), NULL, &height);
	return height;
}

} // namespace SDL

#endif
