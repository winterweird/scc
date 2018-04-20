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
#include <list>
#include <string>
#include <stdexcept>
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

void gameLoop()
{
	Window window("title", Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_RESIZABLE);
	{
		Renderer renderer = window.makeRenderer(); // we won't use this
		if(!window.hasRenderer()) {
			SDL_Log("hasRenderer() says there's no Renderer");
		} else {
			SDL_Log("hasRenderer(): OK");
		}

		auto weakPtr = window.getRenderer();

		bool quit = false;
		while(!quit) {
			SDL_Event e;
			while(SDL_PollEvent(&e)) {
				if(e.type == SDL_QUIT) {
					quit = true;
				}
			}
			std::shared_ptr<Renderer> sharedPtr = weakPtr.lock();
			sharedPtr->setDrawColor(0x00, 0x00, 0x00, 0xff);
			sharedPtr->clear();
			sharedPtr->present();
		}
	}

	if(window.hasRenderer()) {
		SDL_Log("hasRenderer() says there's still a Renderer");
	} else {
		SDL_Log("hasRenderer(): OK");
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
