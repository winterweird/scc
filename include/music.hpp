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

#ifndef SCC_MUSIC_HPP
#define SCC_MUSIC_HPP

#include <memory>
#include <SDL.h>
#include "config.hpp"

#ifndef HAVE_SDL_MIXER
# error "cannot use Music class without SDL_mixer"
#endif

#include <SDL_mixer.h>

namespace SDL {

class RWops;

class Music {
public:
	Music(const char *filename);
	Music(const RWops &file);

	int play(int loops = -1);
	int fadeIn(int loops, int ms);
	int fadeInPos(int loops, int ms, double position);

	Mix_MusicType getType();

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
	Music & operator=(Music that);
	friend void swap(Music &first, Music &second) noexcept;

	struct Deleter {
		void operator()(Mix_Music *music) { Mix_FreeMusic(music); }
	};
private:
	std::unique_ptr<Mix_Music, Deleter> music_;
};

} // namespace SDL

#endif
