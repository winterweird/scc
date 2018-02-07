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
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "GL/common.hpp"
#include "GL/shader.hpp"
using GL::Shader;

Shader::Shader(GLenum type) : id{glCreateShader(type)}
{
	// we don't call glGetError() because errors could have been generated
	// by previous calls
	if(id == 0 || !glIsShader(id)) {
		throw std::runtime_error("shader creation failed.");
	}
}

Shader::Shader() : id{0} {}

Shader::Shader(Shader &&that) : Shader()
{
	swap(*this, that);
}

Shader::~Shader() { glDeleteShader(id); }

void Shader::source(const std::string &s)
{
	auto src = static_cast<const GLchar*>(s.c_str());
	glShaderSource(id, 1, &src, NULL);
}

void Shader::source(const std::vector<std::string> &vs)
{
	size_t size = vs.size();
	std::unique_ptr<const char*[]> ptrarr{new const char*[size]};
	for(int i = 0; i < size; i++) {
		ptrarr[i] = vs[i].c_str();
	}
	auto src = static_cast<const GLchar**>(ptrarr.get());
	glShaderSource(id, size, src, NULL);
}

void Shader::source(std::ifstream &inFile)
{
	inFile.seekg(0, std::ifstream::end);
	auto size = static_cast<size_t>(inFile.tellg());

	inFile.seekg(0, std::ifstream::beg);

	// there must be a way of doing this without a buffer...
	std::unique_ptr<GLchar[]> buffer{new GLchar[size + 1]};
	inFile.read(buffer.get(), size);
	buffer[size + 1] = '\0';

	auto src = static_cast<const GLchar*>(buffer.get());
	glShaderSource(id, 1, &src, NULL);
}
