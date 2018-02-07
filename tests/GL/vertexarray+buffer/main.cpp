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

#include <vector>
#include <string>
#include <fstream>
#include <glew.h>
#include <gl.h>
#include <SDL.h>
#include "config.hpp"
#include "window.hpp"
#include "glcontext.hpp"
#include "GL/vertexarray.hpp"
#include "GL/buffer.hpp"
using SDL::Window;
using SDL::GLContext;
using GL::VertexArray;
using GL::Buffer;

const int ERR_SDL_INIT = -1;

const GLchar vertexShaderSrc[] =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n"
"}\n";

const GLchar fragShaderSrc[] =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"	color = vec4(0.5f, 0.75f, 0.5f, 1.0f);\n"
"}\n";

const GLfloat vert[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

bool init(Uint32 sdlInitFlags)
{
	return SDL_Init(sdlInitFlags) == 0;
}

void quit()
{
	SDL_Quit();
}

void setContextFlags()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
}

bool initglew()
{
	glewExperimental = GL_TRUE;
	GLenum glewerr = glewInit();
	if(glewerr != GLEW_OK) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"glewInit() failed: %s\n",
			glewGetErrorString(glewerr));
		return false;
	}
	return true;
}

GLuint shaderSetup(const GLchar *src, GLenum type)
{
	GLuint shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &src, NULL);
	glCompileShader(shaderID);

	// shader compilation errors weren't reported on the debug callback of
	// my openGL implementation
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		SDL_Log("shader compilation failed. Shader type: 0x%x\n", type);
	}

	return shaderID;
}

GLuint shaderProgSetup(const GLchar *vertexShaderSrc,
	const GLchar *fragShaderSrc)
{
	GLuint vertexShader = shaderSetup(vertexShaderSrc, GL_VERTEX_SHADER);
	GLuint fragShader = shaderSetup(fragShaderSrc, GL_FRAGMENT_SHADER);

	GLuint shaderProg = glCreateProgram();

	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragShader);

	glLinkProgram(shaderProg);

	// linking error weren't reported on the debug callback either
	GLint success;
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
	if(success == GL_FALSE) {
		SDL_Log("linking shader program failed\n");
	}

	glDetachShader(shaderProg, vertexShader);
	glDetachShader(shaderProg, fragShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return shaderProg;
}

void setupGLObjects(VertexArray &vao, Buffer &vbo)
{
	vao.bind();
	vbo.bind(GL_ARRAY_BUFFER);

	Buffer::data(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	VertexArray::attribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (GLvoid*)0);
	vao.enableAttrib(0);

	vbo.unbind(GL_ARRAY_BUFFER);
	vao.unbind();
}

void gameLoop()
{
	Window window("test", Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_OPENGL);

	setContextFlags();
	GLContext context = window.makeGLContext();

	if(!initglew()) { return; }


	if(SDL_GL_SetSwapInterval(1) < 0) {
		SDL_Log("vsync not supported");
	}

	GLuint shaderProg = shaderProgSetup(vertexShaderSrc, fragShaderSrc);
	VertexArray vao(1);
	Buffer vbo(1);
	setupGLObjects(vao, vbo);

	glUseProgram(shaderProg);

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}
		glClearColor(0.25f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		vao.unbind();

		window.swapWindow();
	}
	glDeleteProgram(shaderProg);
}

int main(int argc, char **argv)
{
	if(!init(SDL_INIT_VIDEO)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL\n");
		return ERR_SDL_INIT;
	}
	gameLoop();
	quit();
	return 0;
}
