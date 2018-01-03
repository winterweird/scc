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

#include <list>
#include <string>
#include <stdexcept>
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

auto getWindowFromID(Uint32 windowID, std::list<Window> &windows)
	-> decltype(windows.begin())
{
	for(auto it = windows.begin(); it != windows.end(); ++it) {
		if(it->getID() == windowID) {
			return it;
		}
	}
	// even though the window is dead there may be events related to it
	// (e.g. hidden, focus lost) so it's not an exception
	return windows.end();
}

void parseWindowEvent(SDL_WindowEvent &we, std::list<Window> &windows)
{
	auto window = getWindowFromID(we.windowID, windows);
	// window iterator may not be dereferenceable, so we get the window ID
	// from the event data.
	Uint32 id = we.windowID;
	if(window == windows.end()) {
		SDL_Log("SDL_WindowEvent associated with dead window. "
			"Window ID: %d, event ID: %d", id, we.event);
	}
	switch(we.event) {
	case SDL_WINDOWEVENT_CLOSE:
		windows.erase(window);
		SDL_Log("window %d closed\n", id);
	break;
	}
}

void parseKeyEvent(SDL_KeyboardEvent &ke, std::list<Window> &windows)
{
	auto focusedWindow = getWindowFromID(ke.windowID, windows);
	bool validFocusedWindow = focusedWindow != windows.end();
	if(!validFocusedWindow) {
		SDL_Log("no window with keyboard focus.\n");
		// not all commands use the focused window, so go on
	}
	switch(ke.keysym.sym) {
	case SDLK_s:
		// show all windows
		// NOTE: this didn't work in Xorg with SDL 2.0.2; it works with
		// version 2.0.7
		for(Window &window : windows) {
			window.show();
		}
	break;
	case SDLK_h:
		// make sure at least one window is shown to recieve the events.
		// windows.begin() may not be the window called "first"
		// (e.g. if you closed it)
		if(validFocusedWindow && focusedWindow != windows.begin()) {
			focusedWindow->hide();
		}
	break;
	case SDLK_r:
		// restore all windows
		// (doesn't work if all windows are minimized, because then none
		// will recieve the key event)
		for(Window &window : windows) {
			window.restore();
		}
	break;
	case SDLK_m:
		if(validFocusedWindow) {
			focusedWindow->minimize();
		}
	break;
	case SDLK_x:
		if(validFocusedWindow) {
			focusedWindow->maximize();
		}
	break;
	case SDLK_f:
		if(validFocusedWindow) {
			// toggle fullscreen
			Uint32 fullscreenFlag = (focusedWindow->getFlags()
				& SDL_WINDOW_FULLSCREEN_DESKTOP)
				? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
			if(!focusedWindow->setFullscreen(fullscreenFlag)) {
				SDL_Log("couldn't set window as fullscreen");
			}
		}
	case SDLK_1:
		if(windows.size() >= 1) {
			auto window1 = windows.begin();

			window1->show();
			window1->raise();
		}
	break;
	case SDLK_2:
		if(windows.size() >= 2) {
			auto window2 = windows.begin();
			window2++;

			window2->show();
			window2->raise();
		}
	break;
	case SDLK_3:
		if(windows.size() >= 3) {
			auto window3 = windows.begin();
			window3++;
			window3++;

			window3->show();
			window3->raise();
		}
	break;
	}
}

void gameLoop()
{
	std::list<Window> windows;
	windows.push_back(Window("first",
		Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_RESIZABLE));
	windows.push_back(Window("second",
		Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_RESIZABLE));
	windows.push_back(Window("third",
		Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_RESIZABLE));

	{
		decltype(windows.begin()) window1, window2, window3;
		window1 = window2 = window3 = windows.begin();
		// ugly, indeed.
		window2++;
		window3++; window3++;

		SDL_Log("window IDs: first = %d, second = %d, third = %d\n",
			window1->getID(), window2->getID(), window3->getID());

		window1->renderer.setDrawColor(0xff, 0x00, 0x00, 0xff); // red
		window2->renderer.setDrawColor(0x00, 0xff, 0x00, 0xff); // green
		window3->renderer.setDrawColor(0x00, 0x00, 0xff, 0xff); // blue
	}

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_WINDOWEVENT) {
				parseWindowEvent(e.window, windows);
			} else if(e.type == SDL_KEYUP) {
				// we use KEYUP to make sure the event is only
				// generated once
				parseKeyEvent(e.key, windows);
			}
		}
		for(auto &it : windows) {
			it.renderer.clear();
			it.renderer.present();
		}
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
