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

const int fixedWidth = 200;
const int fixedHeight = 200;

inline void updateScale(float &xScale, float &yScale,
	int windowWidth, int windowHeight)
{
	xScale = static_cast<float>(windowWidth) /
		static_cast<float>(fixedWidth);
	yScale = static_cast<float>(windowHeight) /
		static_cast<float>(fixedHeight);
}

inline void updateSize(const SDL_Event &e, Renderer &renderer, int &windowWidth,
	int &windowHeight, float &xScale, float &yScale)
{
	float xScaleGot, yScaleGot;
	if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		windowWidth = e.window.data1;
		windowHeight = e.window.data2;

		updateScale(xScale, yScale, windowWidth, windowHeight);

		renderer.setScale(xScale, yScale);
		renderer.getScale(&xScaleGot, &yScaleGot);

		SDL_Log("scale set: x = %f, y = %f", xScale, yScale);
		SDL_Log("scale got: x = %f, y = %f", xScaleGot, yScaleGot);
	}
}

void gameLoop()
{
	int windowWidth = Window::DEFAULT_WIDTH;
	int windowHeight = Window::DEFAULT_HEIGHT;
	Window window("test", windowWidth, windowHeight,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_RESIZABLE);
	Renderer renderer = window.makeRenderer();

	const SDL_Rect rect{fixedWidth / 2 - fixedWidth / 8,
		fixedHeight / 2 - fixedHeight / 8,
		fixedWidth / 4,
		fixedHeight / 4};

	float xScale;
	float yScale;
	updateScale(xScale, yScale, windowWidth, windowHeight);
	renderer.setScale(xScale, yScale);

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_WINDOWEVENT) {
				updateSize(e, renderer, windowWidth, windowHeight,
					xScale, yScale);
			}
		}
		renderer.setDrawColor(0x00, 0x00, 0x00, 0xff);
		renderer.clear();

		renderer.setDrawColor(0xff, 0xff, 0xff, 0xff);
		renderer.fillRect(&rect);

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
