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

// Compile with:
//	g++ -I/usr/include/SDL2 no_SCC_flac_test.cpp -o prog -lSDL2 -lSDL2_mixer
//
// unsurprisingly, this also works with song.ogg but not with song.flac.
//
#include <SDL.h>
#include <SDL_mixer.h>

const char *filename = "song.flac";

int init()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("sdl init error");
		return -1;
	}
	if(Mix_Init(MIX_INIT_FLAC) == 0) {
		SDL_Log("mixer init error");
		return -1;
	}
	if(Mix_OpenAudio(44000, AUDIO_S16LSB, 2, 2048) < 0) {
		SDL_Log("mixer open error");
		return -1;
	}
	return 0;
}

void mainLoop(SDL_Window *window, SDL_Renderer *renderer, Mix_Music *music)
{
	SDL_Event e;
	bool quit = false;
	while(!quit) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
				Mix_PlayMusic(music, 0);
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
}

int main(int argc, char **argv)
{
	if(init() == -1) {
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	if(window == NULL) {
		SDL_Log("window loading error");
		return -1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL) {
		SDL_Log("renderer loading error");
		return -1;
	}

	Mix_Music *music = Mix_LoadMUS(filename);
	if(music == NULL) {
		SDL_Log("music loading error");
		return -1;
	}

	mainLoop(window, renderer, music);

	Mix_FreeMusic(music);
	music = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
}
