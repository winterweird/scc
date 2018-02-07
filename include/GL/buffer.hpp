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

#ifndef SCC_GLBUFFER_HPP
#define SCC_GLBUFFER_HPP

#include <utility>
#include "GL/common.hpp"
#include "GL/object.hpp"

namespace GL {

class Buffer : public Object {
public:
	void bind(GLenum target) override { glBindBuffer(target, id); }
	void unbind(GLenum target) override { glBindBuffer(target, 0); }

	Buffer(GLsizei count);

	static void data(GLenum target, GLsizeiptr size, const GLvoid *data,
		GLenum usage)
	{
		glBufferData(target, size, data, usage);
	}

	static void getParameteriv(GLenum target, GLenum val, GLint *data)
	{
		glGetBufferParameteriv(target, val, data);
	}
	static void getParameteri64v(GLenum target, GLenum val, GLint64 *data)
	{
		glGetBufferParameteri64v(target, val, data);
	}

	Buffer(const Buffer &that) = delete;
	Buffer(Buffer &&that);
	~Buffer();
	Buffer & operator=(Buffer that) { swap(*this, that); return *this; }
	friend void swap(Buffer &first, Buffer &second)
	{
		using std::swap;
		swap(first.id, second.id);
		swap(first.count, second.count);
	}
private:
	Buffer();
	GLuint id;
	GLsizei count;
};

} // namespace GL

#endif // SCC_GLBUFFER_HPP
