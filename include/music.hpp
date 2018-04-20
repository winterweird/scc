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

#ifndef SCC_MUSIC_HPP
#define SCC_MUSIC_HPP

#include <memory>
#include <stdexcept>
#include "null.hpp"
#include "cstylealloc.hpp"
#include "rwops.hpp"

#ifndef HAVE_SDL_MIXER
# error "cannot use Music class without SDL_mixer"
#endif

namespace SDL {

class RWops;

class Music {
public:
	Music(const char *filename) : Music(RWops(filename, "rb")) {}
	Music(const RWops &file);

	int play(int loops = -1) { return Mix_PlayMusic(music_.get(), loops); }
	int fadeIn(int loops, int ms)
	{
		return Mix_FadeInMusic(music_.get(), loops, ms);
	}
	int fadeInPos(int loops, int ms, double position)
	{
		return Mix_FadeInMusicPos(music_.get(), loops, ms, position);
	}

	Mix_MusicType getType() { return Mix_GetMusicType(music_.get()); }

	static int fadeOut(int ms) { return Mix_FadeOutMusic(ms); }
	static int halt() { return Mix_HaltMusic(); }

	static void pause() { Mix_PauseMusic(); }
	static void resume() { Mix_ResumeMusic(); }
	static void rewind() { Mix_RewindMusic(); }

	static bool isPaused() { return Mix_PausedMusic(); }
	static bool isPlaying() { return Mix_PlayingMusic(); }

	static int setVolume(int volume) { return Mix_VolumeMusic(volume); }
	static int getVolume() { return setVolume(-1); }

	static int setPosition(double position)
	{
		return Mix_SetMusicPosition(position);
	}

	Music(const Music &that) = delete;
	Music(Music &&that) = default;
	~Music() = default;
	Music & operator=(Music that) { swap(*this, that); return *this; }
	friend void swap(Music &first, Music &second) noexcept
	{
		using std::swap;
		swap(first.music_, second.music_);
	}

	struct Deleter {
		void operator()(Mix_Music *music) { Mix_FreeMusic(music); }
	};
private:
	std::unique_ptr<Mix_Music, Deleter> music_;
};

Music::Music(const RWops &file)
	: music_{FromRWops<Music::Deleter>::load(file, Mix_LoadMUS_RW,
		"Loading music from file failed")}
{}

} // namespace SDL

#endif
