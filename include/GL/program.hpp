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

#ifndef SCC_GLPROGRAM_HPP
#define SCC_GLPROGRAM_HPP

#include <initializer_list>
#include <functional> // reference_wrapper
#include "GL/common.hpp"

namespace GL {

class Shader;

// in the OpenGL sense; not an actual executable
class Program {
public:
	Program(); // create only

	// also attach the shaders, link the program and detach them
	Program(std::initializer_list<std::reference_wrapper<Shader>> shaders);

	// note: even though attach() doesn't change its argument, it cannot
	// accept a const lvalue reference, as that would allow passing rvalues,
	// which might go out of scope right after this call: the Shader's dtor
	// would be called, and glDeleteShader() with it.
	// But glDeleteShader() defers the deletion until the shader is no
	// longer attached to any program, and we wouldn't be able to call
	// detach() with that Shader instance because it went out of scope.
	// This means attach() would effectively leak memory until this
	// Program instance was deleted.

	// Make sure to call detach() before the Shader goes out of scope!
	void attach(Shader &shader);
	void detach(const Shader &shader);

	void link() { glLinkProgram(id); }

	void use() { glUseProgram(id); }
	static void disuse() { glUseProgram(0); }

	Program(const Program &that) = delete;
	Program(Program &&that);
	~Program();
	Program & operator=(Program that) { swap(*this, that); return *this; }
	friend void swap(Program &first, Program &second)
	{
		using std::swap;
		swap(first.id, second.id);
	}
private:
	GLuint id;
	Program(bool whatever); // empty ctor: arg's value is irrelevant
};

} // namespace GL

#endif // SCC_GLPROGRAM_HPP
