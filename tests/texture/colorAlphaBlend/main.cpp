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

Uint8 step = 0x10;

inline void parseKey(SDL_Keycode key, Uint16 mod, Texture &texture)
{
	bool shiftPressed = (mod & KMOD_SHIFT) != 0;
	bool rgbChanged = false;
	bool blendModeChanged = false;

	Uint8 r, g, b, a;
	SDL_BlendMode blendMode;
	texture.getColorMod(&r, &g, &b);
	texture.getAlphaMod(&a);
	// this isn't really needed, but it's a good failsafe: if we set
	// blendModeChanged incorrectly, the blend mode will be changed to the
	// previous one
	texture.getBlendMode(&blendMode);

	switch(key) {
	case SDLK_r:
	{
		Uint8 oldR = r;
		if(shiftPressed) {
			r += step;
		} else {
			r -= step;
		}
		SDL_Log("old red = %d, new red = %d", oldR, r);
		rgbChanged = true;
	}
	break;
	case SDLK_g:
	{
		Uint8 oldG = g;
		if(shiftPressed) {
			g += step;
		} else {
			g -= step;
		}
		SDL_Log("old green = %d, new green = %d", oldG, g);
		rgbChanged = true;
	}
	break;
	case SDLK_b:
	{
		Uint8 oldB = b;
		if(shiftPressed) {
			b += step;
		} else {
			b -= step;
		}
		SDL_Log("old blue = %d, new blue = %d", oldB, b);
		rgbChanged = true;
	}
	break;
	case SDLK_a:
	{
		Uint8 oldA = a;
		if(shiftPressed) {
			a += step;
		} else {
			a -= step;
		}
		SDL_Log("old alpha = %d, new alpha = %d", oldA, a);
		if(!texture.setAlphaMod(a)) {
			SDL_Log("setAlphaMod() failed");
		}
	}
	break;
	case SDLK_n: // None
		blendMode = SDL_BLENDMODE_NONE;
		blendModeChanged = true;
	break;
	case SDLK_l: // bLend
		blendMode = SDL_BLENDMODE_BLEND;
		blendModeChanged = true;
	break;
	case SDLK_d: // aDd
		blendMode = SDL_BLENDMODE_ADD;
		blendModeChanged = true;
	break;
	case SDLK_m: // Mod
		blendMode = SDL_BLENDMODE_MOD;
		blendModeChanged = true;
	break;

	}

	if(rgbChanged) {
		if(!texture.setColorMod(r, g, b)) {
			SDL_Log("setColorMod() failed");
		}
	}
	if(blendModeChanged) {
		if(!texture.setBlendMode(blendMode)) {
			SDL_Log("setBlendMode() failed");
		}
	}
}

void gameLoop()
{
	Window window("test");
	window.makeRenderer();

	Texture background = window.renderer->makeTexture("background.jpg");
	Texture foreground = window.renderer->makeTexture("foreground.jpg");

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_KEYDOWN) {
				parseKey(e.key.keysym.sym, e.key.keysym.mod,
					foreground);
			}
		}

		window.renderer->setDrawColor(0, 0, 0, 0xff);
		window.renderer->clear();

		window.renderer->render(background);
		window.renderer->render(foreground);

		window.renderer->present();
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
