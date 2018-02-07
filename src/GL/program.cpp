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
#include "GL/common.hpp"
#include "GL/shader.hpp"
#include "GL/program.hpp"
using GL::Shader;
using GL::Program;

Program::Program() : id{glCreateProgram()}
{
	if(id == 0 || !glIsProgram(id)) {
		throw std::runtime_error("GL::Program creation failed");
	}
}

Program::Program(std::initializer_list<std::reference_wrapper<Shader>> shaders)
	: Program()
{
	for(auto it : shaders) { attach(it.get()); }
	link();
	for(auto it : shaders) { detach(it.get()); }
}

Program::Program(bool whatever) : id{0} {}

Program::Program(Program &&that) : Program(false)
{
	swap(*this, that);
}

Program::~Program() { glDeleteProgram(id); }

// note: attach() and detach() are simple enough to have been defined in the
// header, but that would require the complete definiton of Shader to be there
// (rather than just a forward delcaration)
//
void Program::attach(Shader &shader)
{
	glAttachShader(id, shader.id);
}

void Program::detach(const Shader &shader)
{
	glDetachShader(id, shader.id);
}
