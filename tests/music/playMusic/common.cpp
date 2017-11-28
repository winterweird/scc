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
#include <SDL_mixer.h>
#include "music.hpp"
using SDL::Music;

const int FREQUENCY = 44100;
const Uint16 FORMAT = AUDIO_U16LSB;
const int CHANNELS = 2; // stereo
const int CHUNK_SIZE = 2048;

const int FADE_INTERVAL = 4000; // ms
const int VOLUME_STEP = 16;

void parseEvent(SDL_Event &ev, Music &music, bool &quit)
{
	// TODO setPosition() (left and right arrows)
	if(ev.type == SDL_KEYDOWN) {
		switch(ev.key.keysym.sym) {
		case SDLK_0:
			music.rewind();
		break;
		case SDLK_h:
			music.halt();
		break;
		case SDLK_UP:
		{
			int vol = music.getVolume();
			SDL_Log("old volume: %d\n", vol);
			vol += VOLUME_STEP;
			if(vol > MIX_MAX_VOLUME) {
				vol = MIX_MAX_VOLUME;
			}
			music.setVolume(vol);
			SDL_Log("new volume: %d\n", vol);
		}
		break;
		case SDLK_DOWN:
		{
			int vol = music.getVolume();
			SDL_Log("old volume: %d\n", vol);
			vol -= VOLUME_STEP;
			if(vol < 0) {
				vol = 0;
			}
			music.setVolume(vol);
			SDL_Log("new volume: %d\n", vol);
		}
		break;
		case SDLK_i:
			music.fadeIn(1, FADE_INTERVAL);
		break;
		case SDLK_o:
			music.fadeOut(FADE_INTERVAL);
		break;
		case SDLK_p:
			music.play(1); // play once
		break;
		case SDLK_SPACE:
			if(music.isPaused()) {
				SDL_Log("resuming music\n");
				music.resume();
			} else {
				SDL_Log("pausing music\n");
				music.pause();
			}
		break;
		}
	} else if(ev.type == SDL_QUIT) {
		quit = true;
	}
}

void test(const char *filename, Mix_MusicType musicType)
{
	Music music(filename);
	if(music.getType() != musicType) {
		SDL_Log("wrong music type\n");
		// go on, it may be that the type passed to this was wrong
	}

	// raw SDL types are used to prevent test's dependence on more classes
	SDL_Window *window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_PRESENTVSYNC);

	SDL_Event ev;
	bool quit = false;
	while(!quit) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		while(SDL_PollEvent(&ev)) {
			parseEvent(ev, music, quit);
		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
}

bool init(Uint32 sdlInitFlags, int mixerInitFlags)
{
	if(SDL_Init(sdlInitFlags) < 0) {
		return false;
	}
	if((Mix_Init(mixerInitFlags) & mixerInitFlags) != mixerInitFlags) {
		return false;
	}
	if(Mix_OpenAudio(FREQUENCY, FORMAT, CHANNELS, CHUNK_SIZE) < 0) {
		return false;
	}
	return true;
}

void quit()
{
	Mix_CloseAudio();
	while(Mix_Init(0)) {
		Mix_Quit();
	}
	SDL_Quit();
}
