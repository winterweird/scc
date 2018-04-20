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

#ifndef SCC_RWOPS_HPP
#define SCC_RWOPS_HPP

#include <memory>
#include "null.hpp"
#include "cstylealloc.hpp"

namespace SDL {

class RWops {
	template <typename Deleter> friend struct FromRWops; // defined below
public:
	RWops(const char *filename, const char *mode); // fromFile
	RWops(void *memory, int size); // fromMem
	RWops(const void *memory, int size); // fromConstMem

	// custom SDL_RWops.
	// Notes:
	// - Doesn't work with lambdas, since SDL wants function pointers
	// - Doesn't work with member functions, since their prototype is
	//   different, but you may wrap them around free functions
	//   (by putting a pointer to the object in one of the data fields)
	// - Make sure your close() calls SDL_FreeRW(), because this is
	//   allocated with SDL_AllocRW()
	// - Make sure whatever the data fields point to isn't deleted while
	//   this object exists. You could do that by making close()
	//   responsible for deallocating what they point to, but if you do
	//   that and the ctor throws, the dtor will never be called
	//   (hence, neither will close()) and you'll have a memory leak.
	RWops(
		decltype(SDL_RWops::size) size,
		decltype(SDL_RWops::seek) seek,
		decltype(SDL_RWops::read) read,
		decltype(SDL_RWops::write) write,
		decltype(SDL_RWops::close) close,
		Uint32 type = SDL_RWOPS_UNKNOWN,
		void *data1 = NULL, // hidden.unknown
		void *data2 = NULL  // hidden.unknown
	);

	// wrappers around SDL_RW* functions. The args and return value are the
	// same as in SDL.
	// There's no wrapper around SDL_RWclose() because that will only be
	// called upon destruction
	Sint64 size() const { return SDL_RWsize(rwops_.get()); }
	size_t read(void *ptr, size_t size, size_t maxnum) const
	{
		return SDL_RWread(rwops_.get(), ptr, size, maxnum);
	}
	size_t write(const void *ptr, size_t size, size_t num)
	{
		return SDL_RWwrite(rwops_.get(), ptr, size, num);
	}
	Sint64 seek(Sint64 offset, int whence)
	{
		return SDL_RWseek(rwops_.get(), offset, whence);
	}
	Sint64 tell() const { return SDL_RWtell(rwops_.get()); }

	RWops(const RWops &that) = delete;
	RWops(RWops &&that) = default;
	~RWops() = default;
	RWops & operator=(RWops that) { swap(*this, that); return *this; }
	friend void swap(RWops &first, RWops &second) noexcept
	{
		using std::swap;
		swap(first.rwops_, second.rwops_);
	}

	struct Deleter {
		void operator()(SDL_RWops *rwops) { SDL_RWclose(rwops); }
	};
private:
	std::unique_ptr<SDL_RWops, Deleter> rwops_;
};

// Class that actually allows loading from RWops, while ensuring freesrc will
// always be false (only the dtor should free the SDL_RWops)
//
// Note: once again, Deleter type can't be automatically deduced, so we put it
// in a class
template <typename Deleter>
struct FromRWops {
	// Note: this assumes the function's signature is
	//	f(SDL_RWops *rwops, int freesrc, Args ... args)
	// Meaning any extra argument that the function takes has to go
	// after the rwops pointer (otherwise, use std::bind or a lambda).
	// freesrc is assumed to mean whether or not rwops should be freed,
	// and 0 means it shouldn't.
	template<typename F, typename ... Args>
	static auto load(const RWops& rwops, F f, const char *errorMsg,
		Args&& ... args)
	-> decltype(CStyleAlloc<Deleter>::alloc(std::declval<F>(),
		std::declval<const char*>(), std::declval<SDL_RWops*>(),
		std::declval<int>(), std::declval<Args>()...))
	{
		return CStyleAlloc<Deleter>::alloc(f, errorMsg,
			rwops.rwops_.get(), 0, std::forward<Args>(args)...);
	}
};

RWops::RWops(const char *filename, const char *mode)
	: rwops_{CStyleAlloc<RWops::Deleter>::alloc(SDL_RWFromFile,
		"Making RWops from file failed", filename, mode)}
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

} // namespace SDL

#endif // SCC_RWOPS_HPP
