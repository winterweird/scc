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
#include "config.hpp"
#include "window.hpp"
using SDL::Window;

const int ERR_SDL_INIT = -1;

bool init(Uint32 sdlInitFlags)
{
	return SDL_Init(sdlInitFlags) == 0;
}

void quit()
{
	SDL_Quit();
}

const Uint8 alphaStep = 0x10;

inline void parseKey(SDL_Keycode key, Uint8 &alpha)
{
	switch(key) {
	case SDLK_MINUS:
		alpha -= alphaStep;
	break;
	case SDLK_EQUALS:
		alpha += alphaStep;
	break;
	}
}

void gameLoop()
{
	Window window("test");
	const int windowWidth = window.getWidth();
	const int windowHeight = window.getHeight();
	const SDL_Rect rect{windowWidth / 4, windowHeight / 4,
		windowWidth / 2, windowHeight / 2};

	Uint8 alpha = 0xff;
	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_KEYDOWN) {
				parseKey(e.key.keysym.sym, alpha);
			}
		}
		window.renderer.setDrawColor(0xff, 0xff, 0xff, 0xff);
		window.renderer.clear();

		window.renderer.setDrawBlendMode(SDL_BLENDMODE_BLEND);
		window.renderer.setDrawColor(0x00, 0x00, 0x00, alpha);
		window.renderer.fillRect(&rect);

		window.renderer.present();
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
