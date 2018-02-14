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

#include <stdexcept>
#include "surface.hpp"
#include "GL/common.hpp"
#include "GL/gltexture.hpp"
using GL::Texture;
using SDL::Surface;

Texture::Texture() : id{0}, count{0}
{}

Texture::Texture(GLsizei count) : count{count}
{
	glGenTextures(count, &id);
	if(id == 0) {
		throw std::runtime_error("GL::Texture creation failed.");
	}
}

Texture::Texture(Texture &&that) : Texture()
{
	swap(*this, that);
}

Texture::~Texture() { glDeleteTextures(count, &id); }

inline GLenum mapPixelFormatSDL2GL(Uint32 informat)
{
	GLenum outformat;
	switch(informat) {
	case SDL_PIXELFORMAT_RGBA8888:
		outformat = GL_RGBA;
	break;
	case SDL_PIXELFORMAT_BGRA8888:
		outformat = GL_BGRA;
	break;
	case SDL_PIXELFORMAT_RGB888:
	case SDL_PIXELFORMAT_RGB24:
		outformat = GL_RGB;
	break;
	case SDL_PIXELFORMAT_BGR888:
	case SDL_PIXELFORMAT_BGR24:
		outformat = GL_BGR;
	break;
	default:
		outformat = GL_RGBA;
	}
	return outformat;
}

void Texture::image2D(GLenum target, const Surface &surface,
	GLenum internalFormat, GLenum type, GLint level)
{
	image2D(target, level, internalFormat,
		surface.getWidth(), surface.getHeight(), 0,
		mapPixelFormatSDL2GL(surface.getPixelFormat()),
		type, surface.getPixels());
}
