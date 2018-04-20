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
#include <SDL_mixer.h>
#include "audiochannels.hpp"
#include "audiochunk.hpp"
using SDL::AudioChunk;
using SDL::AudioChannels;

#include "notMine.hpp"

// note: I've had to change this wav's bit depth from 24 bit to 8 bit, to get it
// working with SDL_mixer
const char *keyWav = "keys.wav";
// note: I've had to change this wav from 5.1 to stereo, to get it working
const char *switchWav = "switch.wav";

// note: these values are the same as the wav file that we want to quickload.
// $ file pop.wav
// pop.wav: RIFF (little-endian) data, WAVE audio, Microsoft PCM, 16 bit,
// stereo 44100 Hz
//
const int FREQUENCY = 44100;
const Uint16 FORMAT = AUDIO_U16LSB;
const int CHANNELS = 2; // stereo
const int CHUNK_SIZE = 2048; // not dependent on the wave file

const int DEFAULT_CHANNEL = -1; // first channel available
const int VOLUME_STEP = 20;
const int FADE_INTERVAL = 20; // ms

inline void volumeUp(AudioChunk *selected)
{
	int vol = selected->getVolume();
	SDL_Log("old volume: %d\n", vol);
	vol += VOLUME_STEP;
	if(vol > MIX_MAX_VOLUME) {
		vol = MIX_MAX_VOLUME;
	}
	selected->setVolume(vol);
	SDL_Log("new volume: %d\n", vol);
}

inline void volumeDown(AudioChunk *selected)
{
	int vol = selected->getVolume();
	SDL_Log("old volume: %d\n", vol);
	vol -= VOLUME_STEP;
	if(vol < 0) {
		vol = 0;
	}
	selected->setVolume(vol);
	SDL_Log("new volume: %d\n", vol);
}

inline void play(AudioChunk *selected, int channel, int loops)
{
	selected->play(channel, loops);
}

inline void fadeIn(AudioChunk *selected, int channel, int loops, int ms)
{
	selected->fadeIn(channel, loops, ms);
}

void parseEvent(SDL_Event &ev, AudioChunk &chunk1, AudioChunk &chunk2,
	AudioChunk &chunk3, bool &quit)
{
	// static so it's not reset to chunk1 every time
	static AudioChunk *selected = &chunk1;
	if(ev.type == SDL_KEYDOWN) {
		switch(ev.key.keysym.sym) {
		case SDLK_1:
			selected = &chunk1;
		break;
		case SDLK_2:
			selected = &chunk2;
		break;
		case SDLK_3:
			selected = &chunk3;
		break;
		case SDLK_UP:
			volumeUp(selected);
		break;
		case SDLK_DOWN:
			volumeDown(selected);
		break;
		case SDLK_p:
			play(selected, DEFAULT_CHANNEL, 0); // play once
		break;
		case SDLK_l:
			play(selected, DEFAULT_CHANNEL, -1); // play on loop
		break;
		case SDLK_i:
			// indistinguishable from play() with the wav's used
			fadeIn(selected, DEFAULT_CHANNEL, 0, FADE_INTERVAL);
		break;
		case SDLK_h:
			AudioChannels::halt(DEFAULT_CHANNEL);
		break;
		case SDLK_o:
			// indistinguishable from halt() with the wav's used
			AudioChannels::fadeOut(DEFAULT_CHANNEL, FADE_INTERVAL);
		break;
		case SDLK_SPACE:
			if(AudioChannels::isPaused(DEFAULT_CHANNEL)) {
				AudioChannels::resume(DEFAULT_CHANNEL);
			} else {
				AudioChannels::pause(DEFAULT_CHANNEL);
			}
		break;
		}
	} else if(ev.type == SDL_QUIT) {
		quit = true;
	}
}

void test()
{
	AudioChunk keyChunk(keyWav); // load wav from file
	AudioChunk popChunk(popWav); // quickload wav from memory

	Mix_Chunk *chunkHolder = Mix_LoadWAV(switchWav);
	// quickload from raw data, conveniently initialized above
	AudioChunk switchChunk(chunkHolder->abuf, chunkHolder->alen);

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
			parseEvent(ev, keyChunk, popChunk, switchChunk, quit);
		}

		SDL_RenderPresent(renderer);
	}

	// note: we free chunkHolder normally, because it marked its buffer as
	// allocated, while switchChunk didn't (no double free)
	Mix_FreeChunk(chunkHolder);
	chunkHolder = NULL;
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

const int ERR_SDL_INIT = -1;

int main(int argc, char **argv)
{
	Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	int mixerFlags = 0;
	if(!init(sdlFlags, mixerFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL: %s\n", SDL_GetError());
		return ERR_SDL_INIT;
	}
	test();
	quit();
	return 0;
}
