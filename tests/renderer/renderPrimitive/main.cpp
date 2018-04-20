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

#include <vector>
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
	// setting the window dimensions to something smaller would likely
	// require repositioning of some primitives
	Window window("test", 800, 600);
	Renderer renderer = window.makeRenderer();

	SDL_Rect rect1{0, 200, 100, 200};
	SDL_Rect rect2{450, 275, 100, 50};

	// we could've all of the drawing operations outside of the main loop
	// since they never change, but this way we can also measure CPU usage
	// with this test
	//
	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}

		renderer.setDrawColor(0x00, 0x00, 0x00);
		renderer.clear();

		renderer.setDrawColor(0xff, 0xff, 0xff);
		renderer.drawPoint(400, 150);

		renderer.setDrawColor(0x00, 0xff, 0x00);
		renderer.drawPoints(std::vector<SDL_Point>{
			{50, 100}, {100, 100}, {150, 100}, {200, 100},
			{250, 100}, {300, 100}, {350, 100}, {400, 100},
			{500, 100}, {550, 100}, {600, 100}, {650, 100},
			{700, 100}
		});

		renderer.setDrawColor(0x00, 0x00, 0xff);
		renderer.drawLine(0, 0, 800, 600);

		renderer.setDrawColor(0xff, 0x00, 0x00);
		renderer.drawLines(std::vector<SDL_Point>{
			{200, 200}, {200, 400}, {350, 300}, {200, 200}
		});

		renderer.setDrawColor(0xff, 0xff, 0x00);
		renderer.drawRect(&rect1);

		renderer.setDrawColor(0x00, 0xff, 0xff);
		renderer.drawRects(std::vector<SDL_Rect>{
			{650, 200, 100, 50},
			{700, 250, 100, 50},
			{650, 300, 100, 50},
			{700, 350, 100, 50}
		});

		renderer.setDrawColor(0xff, 0x00, 0xff);
		renderer.fillRect(&rect2);

		renderer.setDrawColor(0x72, 0x2f, 0x37); // wine
		renderer.fillRects(std::vector<SDL_Rect>{
			{600, 200, 50, 50},
			{750, 200, 50, 50},
			{600, 250, 100, 50},
			{600, 300, 50, 50},
			{750, 300, 50, 50},
			{600, 350, 100, 50}
		});

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
