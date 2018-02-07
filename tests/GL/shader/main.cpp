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
#include "GL/shader.hpp"
using SDL::Window;
using SDL::GLContext;
using GL::Shader;

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

// we can set the source as anything, so long as we don't compile it

const std::string eureka {
	"Proposition 5. Any solid lighter than a fluid will, if placed in the "
	"fluid, be so far immersed that the weight of the solid will be equal "
	"to the weight of the fluid displaced.\n"
	"(Archimedes of Syracuse, circa 250 BC)"
};

const std::vector<std::string> principia {
	"Law I. Every body preserves in its state of rest, or of uniform "
	"motion in a right line, unless it is compelled to change that state "
	"by forces impressed thereon.\n",

	"Law II. The alteration of motion is ever proportional to the motive "
	"force impressed; and is made in the diretion of the right line in "
	"which that force is impressed.\n",

	"Law III. To every action there is always opposed an equal reaction: "
	"or the mutual actions of two bodies upon each other are always "
	"equal, and directed to contrary parts.\n",

	"(Isaac Newton, 1687)"
};

std::ifstream emc2{"energy-mass.txt"};

// a valid source now, we need to test compilation too
const std::string vertexShaderSrc {
	"#version 330 core\n"
	"layout (location = 0) in vec3 pos; \n"
	"void main() { gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); }\n"
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

void logSource(Shader &shader, GLsizei bufsiz, GLsizei *length, GLchar *src)
{
	shader.getSource(bufsiz, length, src);
	SDL_Log("shader source size: %d; shader source:\n'%s'\n", *length, src);
}

void gameLoop()
{
	Window window("test", Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_OPENGL);

	setContextFlags();
	GLContext context = window.makeGLContext();

	if(!initglew()) { return; }

	Shader pseudoShader(GL_VERTEX_SHADER);
	GLchar sourceGot[bufsiz];
	GLsizei lengthGot;

	pseudoShader.source(eureka);
	logSource(pseudoShader, bufsiz, &lengthGot, sourceGot);

	pseudoShader.source(principia);
	logSource(pseudoShader, bufsiz, &lengthGot, sourceGot);

	pseudoShader.source(emc2);
	logSource(pseudoShader, bufsiz, &lengthGot, sourceGot);

	// an actual shader now
	Shader vertexShader(GL_VERTEX_SHADER, vertexShaderSrc);

	GLint success;
	vertexShader.getiv(GL_COMPILE_STATUS, &success);
	if(success) {
		SDL_Log("shader compilation succeeded.");
	} else {
		SDL_Log("shader compilation failed.");
	}

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		window.swapWindow();
	}
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
