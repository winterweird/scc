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

#include <SDL.h>
#include "window.hpp"
#include "renderer.hpp"
using SDL::Window;
using SDL::Renderer;

const int ERR_SDL_INIT = -1;

bool init(Uint32 sdlInitFlags)
{
	return SDL_Init(sdlInitFlags) == 0;
}

void quit()
{
	SDL_Quit();
}

inline void parseWindowEvent(const SDL_Event &e, Window &window)
{
	// this one is guaranteed every time the window changes, unlinke
	// SDL_WINDOW_EVENT_RESIZED
	if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		SDL_Log("(SDL_event data) new size:\t%d x %d\n",
			e.window.data1, e.window.data2);
		SDL_Log("(SDL::Window data) new size:\t%d x %d\n",
			window.getWidth(), window.getHeight());
	}
}

void gameLoop()
{
	Window window("test", Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_RESIZABLE);
	Renderer renderer = window.makeRenderer();

	renderer.setDrawColor(0, 0, 0, 255); // black

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_WINDOWEVENT) {
				parseWindowEvent(e, window);
			}
		}
		renderer.clear();
		renderer.present();
	}
}

int main(int argc, char **argv)
{
	if(!init(SDL_INIT_VIDEO)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL\n");
		return ERR_SDL_INIT;
	}
	gameLoop();
	quit();
	return 0;
}
