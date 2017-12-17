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

void gameLoop()
{
	Window window("test");
	const int windowWidth = window.getWidth();
	const int windowHeight = window.getHeight();
	const SDL_Rect screenRect{0, 0, windowWidth, windowHeight};

	const SDL_Rect viewport1{0, 0,
		windowWidth * 2 / 5, windowHeight * 2 / 5};
	const SDL_Rect viewport2{windowWidth * 3 / 5, 0,
		windowWidth * 2 / 5, windowHeight * 3 / 8};
	const SDL_Rect viewport3{windowWidth * 3 / 5, windowHeight * 3 / 5,
		windowWidth * 2 / 5, windowHeight * 2 / 5};
	const SDL_Rect viewport4{0, windowHeight * 3 / 5,
		windowWidth * 2 / 5, windowHeight * 2 / 5};

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}
		window.renderer.setDrawColor(0x00, 0x00, 0x00, 0xff); // black
		window.renderer.clear();

		// after this, the background color should be red
		window.renderer.setDrawColor(0xff, 0x00, 0x00, 0xff);
		window.renderer.setViewport(nullptr);
		window.renderer.fillRect(&screenRect);

		window.renderer.setDrawColor(0xff, 0xff, 0xff, 0xff); // white

		// all fillRect() calls below shouldn't fill the whole screen,
		// but rather only the viewports
	
		window.renderer.setViewport(&viewport1);
		window.renderer.fillRect(&screenRect);

		window.renderer.setViewport(&viewport2);
		window.renderer.fillRect(&screenRect);

		window.renderer.setViewport(&viewport3);
		window.renderer.fillRect(&screenRect);

		window.renderer.setViewport(&viewport4);
		window.renderer.fillRect(&screenRect);

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
