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

#include <memory>
#include <stdexcept>
#include <vector>
#include <string>
#include <glew.h>
#include <gl.h>
#include <SDL.h>
#include "config.hpp"
#include "window.hpp"
#include "glcontext.hpp"
#include "GL/shader.hpp"
#include "GL/program.hpp"
using SDL::Window;
using SDL::GLContext;
using GL::Shader;
using GL::Program;

const int ERR_SDL_INIT = -1;

bool init(Uint32 sdlInitFlags)
{
	return SDL_Init(sdlInitFlags) == 0;
}

void quit()
{
	SDL_Quit();
}

constexpr GLsizei bufsiz = 1024;

const std::string vertexShaderSrc {
	"#version 330 core \n"
	"layout (location = 0) in vec3 pos; \n"
	"layout (location = 1) in vec3 color;\n"
	"out vec3 vcolor; \n"
	"void main() \n"
	"{ \n"
	"	gl_Position = vec4(pos, 1.0f); \n"
	"	vcolor = color; \n"
	"} \n"
};

const std::string fragShaderSrc {
	"#version 330 core \n"
	"in vec3 vcolor; \n"
	"out vec4 color; \n"
	"void main() { color = vec4(vcolor, 1.0f); } \n"
};

const GLfloat vertdata[] = {
	//     position    |      color
	 0.0f,  0.75f, 0.0f,  1.0f, 0.0f, 0.0f,
	 0.75f, -0.75f, 0.0f,  0.0f, 1.0f, 0.0f,
	-0.75f, -0.75f, 0.0f,  0.0f, 0.0f, 1.0f
};

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

void setupGLObjects(GLuint &vao, GLuint &vbo)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertdata), vertdata, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void gameLoop()
{
	Window window("test", Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_OPENGL);

	setContextFlags();
	GLContext context{window.makeGLContext()};

	if(!initglew()) { return; }

	// first test: create empty program
	try {
		Program shortLived;
	} catch(std::exception &e) {
		SDL_Log("first test failed\n");
		return;
	}

	// unique_ptr so we can delete them sooner
	std::unique_ptr<Shader> vertexShader(new Shader(GL_VERTEX_SHADER,
		vertexShaderSrc));
	std::unique_ptr<Shader> fragShader(new Shader(GL_FRAGMENT_SHADER,
		fragShaderSrc));

	// second test: create actual program
	try {
		Program shortLived({*vertexShader, *fragShader});
	} catch(std::exception &e) {
		SDL_Log("second test failed\n");
		return;
	}

	// now if we're here, all is good; create the actual program.
	// All this is equivalent to
	// 	Program shaderProg({*vertexShader, *fragShader});
	Program shaderProg;
	shaderProg.attach(*vertexShader);
	shaderProg.attach(*fragShader);
	shaderProg.link();
	shaderProg.detach(*vertexShader);
	shaderProg.detach(*vertexShader);

	// we won't need those anymore
	vertexShader.reset(nullptr);
	fragShader.reset(nullptr);

	// don't use GL::VertexArrays and GL::Buffer to reduce this test's
	// dependence on other classes
	GLuint vao, vbo;
	setupGLObjects(vao, vbo);

	if(SDL_GL_SetSwapInterval(1) < 0) {
		SDL_Log("vsync not supported");
	}

	bool quit = false;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}
		glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProg.use();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		Program::disuse();

		window.swapWindow();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
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
