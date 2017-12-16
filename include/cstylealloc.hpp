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

#ifndef SCC_CSTYLEALLOC_HPP
#define SCC_CSTYLEALLOC_HPP

#include <memory>
#include <stdexcept>
#include <utility>
#include <type_traits>

namespace SDL {

// This is meant for SCC internal use, but you can use it as well.
// It gaps the bridge between C and C++ allocators.
// The function it recieves as parameter should try to allocate memory, and
// return NULL (ie, 0) in case of failure.
//
// Notes:
// - you can only use this with actual functions, not with macros like
// SDL_LoadBMP (use the *_RW counterpart instead)
// - the Deleter type cannot be automatically deduced, which is why it's
// been put in a template class, but all other types can, which is why they've
// been put in the template function
//
template <typename Deleter>
struct CStyleAlloc {

	// Notes:
	// - I chose to recieve f as a template parameter, rather than an
	// std::function or similar, because of forwarding issues, namely:
	// what's f's signature? It can't be f(Args ...), because deduction
	// would fail with lvalues (forwarded as lvalue references), and it
	// can't be f(Args&& ...) because that only works if the real f only
	// recieves rvalue references as arguments.
	// The quickest workaround is not to specify f's signature at all.
	//
	// - ideally, this should create an alias to the type T the first time
	// it appears (in the middle of alloc's return type), but I don't think
	// it's possible
	//
	// - we don't check f with std::is_function because that would refuse
	// several viable functions, such as function pointers and lambdas. But
	// f still has to be callable, or else you may get undefined behaviour:
	// "As formulated in C++11, the behavior of std::result_of is undefined
	// when INVOKE(std::declval<F>(), std::declval<ArgTypes>()...) is
	// ill-formed (e.g. when F is not a callable type at all). C++14 changes
	// that to a SFINAE"
	// (<en.cppreference.com/w/cpp/types/result_of>)
	//
	// Lo, and behold: C++ templates in all their majesty!
	//
	template<typename F, typename ... Args>
	static auto alloc(F f, const char *errorMsg, Args&& ... args)
	-> std::unique_ptr<
		typename std::remove_pointer<
			typename std::result_of<F(Args&&...)>::type
		>::type // T
		, Deleter
	>
	{
		auto tmp = f(std::forward<Args>(args)...);
		static_assert(std::is_pointer<decltype(tmp)>::value,
			"CStyleAlloc: f must return a pointer");
		if(tmp == NULL) {
			throw std::runtime_error(errorMsg);
		}
		using T = typename std::remove_pointer<decltype(tmp)>::type;
		return std::unique_ptr<T, Deleter>(tmp);
	}
};

} // namespace SDL

#endif
