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

#ifndef SCC_AUDIOCHUNK_HPP
#define SCC_AUDIOCHUNK_HPP

#include <memory>
#include <stdexcept>
#include "null.hpp"
#include "cstylealloc.hpp"
#include "rwops.hpp"

#ifndef HAVE_SDL_MIXER
# error "cannot use AudioChunk class without SDL_mixer"
#endif

namespace SDL {

class RWops;

class AudioChunk {
	// notes:
	// - To stop playing, call AudioChannels::halt() or
	//   AudioChannels::fadeOut()
	// - You can get the internal pointer managed by this class with
	//   Mix_GetChunk(), but don't.
public:
	AudioChunk(const char *filename) : AudioChunk(RWops(filename, "rb")) {}
	AudioChunk(const RWops &wavFile); // Mix_LoadWAV_RW()
	AudioChunk(Uint8 *mem); // QuickLoad_WAV()
	AudioChunk(Uint8 *mem, Uint32 len); // QuickLoad_RAW()

	int play(int channel, int loops, int ticks = -1)
	{
		return Mix_PlayChannelTimed(channel, chunk_.get(), loops,
			ticks);
	}
	int fadeIn(int channel, int loops, int ms, int ticks = -1)
	{
		return Mix_FadeInChannelTimed(channel, chunk_.get(), loops, ms,
			ticks);
	}
	
	int setVolume(int volume)
	{
		return Mix_VolumeChunk(chunk_.get(), volume);
	}
	int getVolume() { return setVolume(-1); }

	AudioChunk(const AudioChunk &that) = delete;
	AudioChunk(AudioChunk &&that) = default;
	~AudioChunk() = default;
	AudioChunk & operator=(AudioChunk that)
	{
		swap(*this, that);
		return *this;
	}
	friend void swap(AudioChunk &first, AudioChunk &second) noexcept
	{
		using std::swap;
		swap(first.chunk_, second.chunk_);
	}

	struct Deleter {
		void operator()(Mix_Chunk *chunk) { Mix_FreeChunk(chunk); }
	};
private:
	std::unique_ptr<Mix_Chunk, Deleter> chunk_;
};

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

} // namespace SDL

#endif
