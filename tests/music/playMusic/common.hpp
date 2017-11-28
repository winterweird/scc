#ifndef COMMON_HPP
#define COMMON_HPP

#include <SDL.h>
#include <SDL_mixer.h>

void test(const char *filename, Mix_MusicType musicType);
bool init(Uint32 sdlInitFlags, int mixerInitFlags);
void quit();

#endif
