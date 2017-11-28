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
#include <SDL.h>
#include "cstylealloc.hpp"
#include "rwops.hpp"
using SDL::RWops;

RWops::RWops(const char *filename, const char *mode)
	: rwops_{CStyleAlloc<RWops::Deleter>::alloc(SDL_RWFromFile,
		"Making RWops from file", filename, mode)}
{}

RWops::RWops(void *mem, int size)
	: rwops_{CStyleAlloc<RWops::Deleter>::alloc(SDL_RWFromMem,
		"Making RWops from memory failed", mem, size)}
{}

RWops::RWops(const void *mem, int size)
	: rwops_{CStyleAlloc<RWops::Deleter>::alloc(SDL_RWFromConstMem,
		"Making RWops from const memory failed", mem, size)}
{}

RWops::RWops(
	decltype(SDL_RWops::size) size,
	decltype(SDL_RWops::seek) seek,
	decltype(SDL_RWops::read) read,
	decltype(SDL_RWops::write) write,
	decltype(SDL_RWops::close) close,
	Uint32 type,
	void *data1,
	void *data2
) : rwops_{CStyleAlloc<RWops::Deleter>::alloc(SDL_AllocRW,
	"Making custom RWops failed")}
{
	// here, rwops_ already has valid data
	rwops_->size = size;
	rwops_->seek = seek;
	rwops_->read = read;
	rwops_->write = write;
	rwops_->close = close;
	rwops_->type = type;
	rwops_->hidden.unknown.data1 = data1;
	rwops_->hidden.unknown.data2 = data2;
}

RWops & RWops::operator=(RWops that)
{
	swap(*this, that);
	return *this;
}

namespace SDL {

void swap(RWops &first, RWops &second)
{
	using std::swap;
	swap(first.rwops_, second.rwops_);
}

} // namespace SDL

Sint64 RWops::size() const
{
	return SDL_RWsize(rwops_.get());
}

size_t RWops::read(void *ptr, size_t size, size_t maxnum) const
{
	return SDL_RWread(rwops_.get(), ptr, size, maxnum);
}

size_t RWops::write(const void *ptr, size_t size, size_t num)
{
	return SDL_RWwrite(rwops_.get(), ptr, size, num);
}

Sint64 RWops::seek(Sint64 offset, int whence)
{
	return SDL_RWseek(rwops_.get(), offset, whence);
}

Sint64 RWops::tell() const
{
	return SDL_RWtell(rwops_.get());
}
