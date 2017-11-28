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
#include "music.hpp"
using SDL::Music;

Music::Music(const char *filename) : Music(RWops(filename, "rb"))
{}

Music::Music(const RWops &file)
	: music_{FromRWops<Music::Deleter>::load(file, Mix_LoadMUS_RW,
		"Loading music from file failed")}
{}

int Music::play(int loops)
{
	return Mix_PlayMusic(music_.get(), loops);
}

int Music::fadeIn(int loops, int ms)
{
	return Mix_FadeInMusic(music_.get(), loops, ms);
}

int Music::fadeInPos(int loops, int ms, double position)
{
	return Mix_FadeInMusicPos(music_.get(), loops, ms, position);
}

Mix_MusicType Music::getType()
{
	return Mix_GetMusicType(music_.get());
}

Music & Music::operator=(Music that)
{
	swap(*this, that);
	return *this;
}

namespace SDL {

void swap(Music &first, Music &second) noexcept
{
	using std::swap;
	swap(first.music_, second.music_);
}

} // namespace SDL
