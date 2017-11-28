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
#include <SDL_image.h>
#include "config.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "texture.hpp"

const int ERR_SDL_INIT = -1;
const char *imagePath = "foo.jpg";

bool init(Uint32 sdlInitFlags, Uint32 imgInitFlags)
{
	if(SDL_Init(sdlInitFlags) < 0) {
		return false;
	}
	if(IMG_Init(imgInitFlags) != imgInitFlags) {
		return false;
	}
	return true;
}

void quit()
{
	SDL_Quit();
	IMG_Quit();
}

void gameLoop()
{
	SDL::Window window("test");
	SDL::Texture imgTexture = window.renderer.makeTexture(imagePath);

	SDL_Log("texture width: %d", imgTexture.getWidth());
	SDL_Log("texture height: %d", imgTexture.getHeight());

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}
		window.renderer.setRenderDrawColor(255, 255, 255, 255);
		window.renderer.renderClear();

		imgTexture.render(
			window.renderer,
			(window.getWidth() - imgTexture.getWidth()) / 2,
			(window.getHeight() - imgTexture.getHeight()) / 2
		);

		window.renderer.renderPresent();
	}
}

int main(int argc, char **argv)
{
	Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
	Uint32 imgFlags = IMG_INIT_JPG | IMG_INIT_PNG; 
	if(!init(sdlFlags, imgFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL\n");
		return ERR_SDL_INIT;
	}
	gameLoop();
	quit();
	return 0;
}
