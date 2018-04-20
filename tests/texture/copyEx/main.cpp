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

#include <SDL.h>
#include <SDL_image.h>
#include "window.hpp"
#include "renderer.hpp"
#include "texture.hpp"

using SDL::Window;
using SDL::Renderer;
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

const char *imgName = "image.jpg";
const double angleStep = 10.0;

inline void parseKey(SDL_Keycode key, double &angle, SDL_RendererFlip &flip)
{
	switch(key) {
	case SDLK_MINUS:
		angle += angleStep;
	break;
	case SDLK_EQUALS:
		angle -= angleStep;
	break;
	case SDLK_0:
		angle = 0.0;
	break;
	case SDLK_h:
		flip = static_cast<SDL_RendererFlip>(flip ^ SDL_FLIP_HORIZONTAL);
	break;
	case SDLK_v:
		flip = static_cast<SDL_RendererFlip>(flip ^ SDL_FLIP_VERTICAL);
	break;
	case SDLK_n:
		flip = SDL_FLIP_NONE;
	break;
	}
}

void gameLoop()
{
	Window window("test");
	Renderer renderer = window.makeRenderer();

	const int windowWidth = window.getWidth();
	const int windowHeight = window.getHeight();

	Texture texture = renderer.makeTexture(imgName);
	double angle = 0.0;
	SDL_Point center{windowWidth / 2, windowHeight / 2};
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_KEYDOWN) {
				parseKey(e.key.keysym.sym, angle, flip);
			}
		}

		renderer.setDrawColor(0xff, 0xff, 0xff, 0xff);
		renderer.clear();

		renderer.render(texture, NULL, NULL, angle, &center, flip);

		renderer.present();
	}
}

int main(int argc, char **argv)
{
	Uint32 sdlFlags = SDL_INIT_VIDEO;
	if(!init(sdlFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL\n");
		return ERR_SDL_INIT;
	}
	gameLoop();
	quit();
	return 0;
}
