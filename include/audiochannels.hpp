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

#ifndef SCC_AUDIO_CHANNELS
#define SCC_AUDIO_CHANNELS

#include "null.hpp"

#ifndef HAVE_SDL_MIXER
# error "cannot use class AudioChannels without SDL_mixer"
#endif

namespace SDL {

// TODO sound effects
//
// convenience wrapper around channel-related functions.
struct AudioChannels {
	static int allocate(int num) { return Mix_AllocateChannels(num); }
	static int reserve(int num) { return Mix_ReserveChannels(num); }

	static int group(int which, int tag)
	{
		return Mix_GroupChannel(which, tag);
	}
	static int group(int from, int to, int tag)
	{
		return Mix_GroupChannels(from, to, tag);
	}
	static int groupAvailable(int tag) { return Mix_GroupAvailable(tag); }
	static int groupCount(int tag) { return Mix_GroupCount(tag); }
	static int groupOldest(int tag) { return Mix_GroupOldest(tag); }
	static int groupNewer(int tag) { return Mix_GroupNewer(tag); }

	// clearer than groupCount(-1).
	static int total() { return Mix_GroupCount(-1); }

	static int halt(int which) { return Mix_HaltChannel(which); }
	static int haltGroup(int tag) { return Mix_HaltGroup(tag); }

	static int expireChannel(int which, int ticks)
	{
		return Mix_ExpireChannel(which, ticks);
	}

	static int fadeOut(int which, int ms)
	{
		return Mix_FadeOutChannel(which, ms);
	}
	static int fadeOutGroup(int tag, int ms)
	{
		return Mix_FadeOutGroup(tag, ms);
	}
	static bool isFading(int which) { return Mix_FadingChannel(which); }

	static void pause(int which) { Mix_Pause(which); }
	static void resume(int which) { Mix_Resume(which); }

	static bool isPaused(int which) { return Mix_Paused(which); }
	static bool isPlaying(int which) { return Mix_Playing(which); }
};

} // namespace SDL

#endif
