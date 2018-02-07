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

#ifndef SCC_GLVERTEXARRAY_HPP
#define SCC_GLVERTEXARRAY_HPP

#include <utility>
#include "GL/common.hpp"
#include "GL/object.hpp"

namespace GL {

class VertexArray : public Object {
public:
	// note: these bind() / unbind() overloads ignore their argument
	void bind(GLenum target = 0) override { glBindVertexArray(id); }
	void unbind(GLenum target = 0) override { glBindVertexArray(0); }

	VertexArray(GLsizei count);

	void enableAttrib(GLuint index) { glEnableVertexAttribArray(index); }
	void disableAttrib(GLuint index) { glDisableVertexAttribArray(index); }

	static void attribPointer(GLuint index, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, const GLvoid *pointer)
	{
		glVertexAttribPointer(index, size, type, normalized, stride,
			pointer);
	}
	static void attribIPointer(GLuint index, GLint size, GLenum type,
		GLsizei stride, const GLvoid *pointer)
	{
		glVertexAttribIPointer(index, size, type, stride, pointer);
	}

	// glGetVertexAttrib*v
	void getfv(GLenum pname, GLfloat *params)
	{
		glGetVertexAttribfv(id, pname, params);
	}
	void getdv(GLenum pname, GLdouble *params)
	{
		glGetVertexAttribdv(id, pname, params);
	}
	void getLdv(GLenum pname, GLdouble *params)
	{
		glGetVertexAttribLdv(id, pname, params);
	}
	void getiv(GLenum pname, GLint *params)
	{
		glGetVertexAttribiv(id, pname, params);
	}
	void getIiv(GLenum pname, GLint *params)
	{
		glGetVertexAttribIiv(id, pname, params);
	}
	void getIuiv(GLenum pname, GLuint *params)
	{
		glGetVertexAttribIuiv(id, pname, params);
	}

	VertexArray(const VertexArray &that) = delete;
	VertexArray(VertexArray &&that);
	~VertexArray();
	VertexArray & operator=(VertexArray that) { swap(*this, that); return *this; }
	friend void swap(VertexArray &first, VertexArray &second)
	{
		using std::swap;
		swap(first.id, second.id);
		swap(first.count, second.count);
	}
private:
	VertexArray();
	GLuint id;
	GLsizei count;
};

} // namespace GL

#endif // SCC_GLVERTEXARRAY_HPP
