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

#include <memory>
#include <stdexcept>
#include <SDL.h>
#include "config.hpp"
#include "cstylealloc.hpp"
#include "rwops.hpp"

#include <SDL_mixer.h>
#include "audiochunk.hpp"
using SDL::AudioChunk;

AudioChunk::AudioChunk(const char *filename) : AudioChunk(RWops(filename, "rb"))
{}

AudioChunk::AudioChunk(const RWops &wavFile)
	: chunk_{FromRWops<AudioChunk::Deleter>::load(wavFile, Mix_LoadWAV_RW,
		"Loading audio chunk from file failed")}
{}

AudioChunk::AudioChunk(Uint8 *mem)
	: chunk_{CStyleAlloc<AudioChunk::Deleter>::alloc(Mix_QuickLoad_WAV,
		"Quickloading audio chunk from memory failed", mem)}
{}

AudioChunk::AudioChunk(Uint8 *mem, Uint32 len)
	: chunk_{CStyleAlloc<AudioChunk::Deleter>::alloc(Mix_QuickLoad_RAW,
		"Quickloading raw audio chunk from memory failed", mem, len)}
{}

int AudioChunk::play(int channel, int loops, int ticks)
{
	return Mix_PlayChannelTimed(channel, chunk_.get(), loops, ticks);
}

int AudioChunk::fadeIn(int channel, int loops, int ms, int ticks)
{
	return Mix_FadeInChannelTimed(channel, chunk_.get(), loops, ms, ticks);
}

int AudioChunk::setVolume(int volume)
{
	return Mix_VolumeChunk(chunk_.get(), volume);
}

AudioChunk & AudioChunk::operator=(AudioChunk that)
{
	swap(*this, that);
	return *this;
}

namespace SDL {

void swap(AudioChunk &first, AudioChunk &second) noexcept
{
	using std::swap;
	swap(first.chunk_, second.chunk_);
}

} // namespace SDL
