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

#ifndef SCC_GLSHADER_HPP
#define SCC_GLSHADER_HPP

#include <vector>
#include <string>
#include <fstream>
#include "GL/common.hpp"

namespace GL {

class Shader {
	friend class Program;
public:
	Shader(GLenum type); // create only

	// also set the Shader's source and compile it.
	template<typename ... Args>
	Shader(GLenum type, Args&& ... args) : Shader(type)
	{
		source(std::forward<Args>(args)...);
		compile();
	}

	// even if your shader is in an external file, you can use this
	// overload by having a simple script that makes a header from that
	// file, storing its contents in a string
	void source(const std::string &s);
	// O(vs.size()), not taking the glShaderSource() call into account
	void source(const std::vector<std::string> &vs);
	// possibly the slowest overload, since the file is read at runtime.
	// Use this so you won't have to re-compile with every shaders' change.
	void source(std::ifstream &inFile);

	void source(const GLchar **src, GLsizei count,
		const GLint *length = NULL)
	{
		glShaderSource(id, count, src, length);
	}

	void compile() { glCompileShader(id); }

	void getSource(GLsizei bufsiz, GLsizei *length, GLchar *src)
	{
		glGetShaderSource(id, bufsiz, length, src);
	}
	void getiv(GLenum pname, GLint *params)
	{
		glGetShaderiv(id, pname, params);
	}

	Shader(const Shader &that) = delete;
	Shader(Shader &&that);
	~Shader();
	Shader & operator=(Shader that) { swap(*this, that); return *this; }
	friend void swap(Shader &first, Shader &second)
	{
		using std::swap;
		swap(first.id, second.id);
	}
private:
	Shader();
	GLuint id;
};

} // namespace GL

#endif // SCC_GLSHADER_HPP
