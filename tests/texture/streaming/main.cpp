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

void gameLoop()
{
	Window window("test");
	Renderer renderer = window.makeRenderer();

	const int windowWidth = window.getWidth();
	const int windowHeight = window.getHeight();

	// get first supported format
	SDL_RendererInfo info;
	renderer.getInfo(&info);
	Uint32 format = *(info.texture_formats);
	SDL_Log("texture format: %s", SDL_GetPixelFormatName(format));
	const int textureWidth = 100;
	const int textureHeight = 100;
	Texture streamTexture = renderer.makeTexture(format,
		SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}

		renderer.setDrawColor(0, 0, 0, 0xff);
		renderer.clear();

		int pitch;
		void *lockedPixels;
		const int lockedWidth = textureWidth;
		const int lockedHeight = textureHeight;
		const SDL_Rect lockArea{0, 0, lockedWidth, lockedHeight};

		bool locked = streamTexture.lock(&lockArea, &lockedPixels,
			&pitch);
		if(!locked) {
			SDL_Log("couldn't lock texture: %s\n", SDL_GetError());
			quit = true;
		}

		int row = rand() % textureHeight;
		int col = rand() % textureWidth;
		int pixelSize = pitch / textureWidth; // in bytes
		for(int i = 0; i < pixelSize; i++) {
			// "reinterpret_cast [...] is purely a compiler
			// directive which instructs the compiler to treat the
			// sequence of bits (object representation) of
			// expression as if it had the type new_type"
			// <en.cppreference.com/w/cpp/language/reinterpret_cast>
			// So yes, reinterpret_cast keeps the pointer's value
			unsigned char *bytePtr =
				reinterpret_cast<unsigned char*>(lockedPixels)
				+ row * pitch + col * pixelSize + i;
			// what color this gives might depend on the texture's
			// format; it's white with RGB(A)
			*bytePtr = 0xff;
		}

		streamTexture.unlock();

		renderer.render(streamTexture,
			(windowWidth - textureWidth) / 2,
			(windowHeight - textureHeight) / 2);

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
