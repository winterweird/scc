/*
  SDL C++ Classes
  Copyright (C) 2017-2018 Mateus Carmo M. de F. Barbosa
 
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

// glew.h has to be included before gl.h 
#include <glew.h>
#include <gl.h>

#include <SDL.h>
#include "window.hpp"
#include "glcontext.hpp"

using SDL::Window;
using SDL::GLContext;

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
"	color = vec4(0.5f, 0.75f, 0.5f, 1.0f);\n" // output color is fixed
"}\n";

const GLfloat vertices[] = {
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

// callback to use for opengl errors
void msgCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *msg, const void *userParam)
{
	SDL_Log("openGL message: type = 0x%x, severity = 0x%x, id = %d, message = %s\n",
		type, severity, id, msg);
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

void setupGLObjects(GLuint &vao, GLuint &vbo)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void setContextFlags()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
}

void gameLoop()
{
	Window window("opengl triangle",
		Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_OPENGL);

	setContextFlags();
	window.makeGLContext();

	if(!initglew()) { return; }

	// so we don't have to check for errors every time we call an opengl
	// function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)msgCallbackGL, (void*)0);

	GLuint shaderProg = shaderProgSetup(vertexShaderSrc, fragShaderSrc);
	GLuint vao, vbo;
	setupGLObjects(vao, vbo);

	if(SDL_GL_SetSwapInterval(1) < 0) {
		SDL_LogWarn(SDL_LOG_CATEGORY_ERROR, "vsync not supported\n");
	}

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


		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		window.swapWindow();
	}
	glDeleteProgram(shaderProg);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

int main()
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
