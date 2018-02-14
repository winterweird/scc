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

#ifndef SCC_GLTEXTURE_HPP
#define SCC_GLTEXTURE_HPP

#include "GL/common.hpp"
#include "GL/object.hpp"

namespace SDL { class Surface; }

namespace GL {

class Texture : public Object {
public:
	void bind(GLenum target) override { glBindTexture(target, id); }
	void unbind(GLenum target) override { glBindTexture(target, 0); }

	Texture(GLsizei count); // create only

	static void image2D(GLenum target, GLint level, GLenum internalFormat,
		GLsizei width, GLsizei height, GLint border, GLenum format,
		GLenum type, const GLvoid *data)
	{
		glTexImage2D(target, level, internalFormat, width, height,
			border, format, type, data);
	}
	static void image2D(GLenum target, const SDL::Surface &surface,
		 GLenum internalFormat, GLenum type, GLint level = 0);

	Texture(const Texture &that) = delete;
	Texture(Texture &&that);
	~Texture();
	Texture & operator=(Texture that) { swap(*this, that); return *this; }
	friend void swap(Texture &first, Texture &second)
	{
		using std::swap;
		swap(first.id, second.id);
		swap(first.count, second.count);
	}
private:
	Texture();
	GLuint id;
	GLsizei count;
};

} // namespace GL

#endif // SCC_GLTEXTURE_HPP
