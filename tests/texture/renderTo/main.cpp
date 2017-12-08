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
#include "texture.hpp"

using SDL::Window;
using SDL::Texture;

const int ERR_SDL_INIT = -1;

bool init(Uint32 sdlInitFlags)
{
	if(SDL_Init(sdlInitFlags) < 0) {
		return false;
	}
	return true;
}

void quit()
{
	SDL_Quit();
}

void gameLoop()
{
	Uint32 rendererFlags = SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC
		| SDL_RENDERER_TARGETTEXTURE;

	Window window("test", Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		Window::DEFAULT_INIT_FLAGS, rendererFlags);
	const int windowWidth = window.getWidth();
	const int windowHeight = window.getHeight();

	// get first supported format
	Uint32 format = *(window.renderer.getInfo().texture_formats);
	const int textureWidth = 100;
	const int textureHeight = 100;
	Texture targetTexture = window.renderer.makeTexture(format,
		SDL_TEXTUREACCESS_TARGET, textureWidth, textureHeight);

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}

		window.renderer.setDrawColor(0, 0, 0, 0xff);
		window.renderer.clear();

		bool targetIsSet = window.renderer.setTarget(targetTexture);
		if(!targetIsSet) {
			SDL_Log("setTarget doesn't work: %s", SDL_GetError());
			break;
		}
		window.renderer.setDrawColor(0xff, 0xff, 0xff, 0xff);
		window.renderer.drawLine(textureWidth / 2, 0,
			textureWidth / 2, 100);
		window.renderer.drawLine(0, textureHeight / 2,
			100, textureHeight / 2);

		window.renderer.setTarget(nullptr);
		window.renderer.render(targetTexture,
			(windowWidth - textureWidth) / 2,
			(windowHeight - textureHeight) / 2);

		window.renderer.present();
	}
}

int main(int argc, char **argv)
{
	Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
	if(!init(sdlFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL\n");
		return ERR_SDL_INIT;
	}
	gameLoop();
	quit();
	return 0;
}
