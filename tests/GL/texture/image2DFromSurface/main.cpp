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
#include <glew.h>
#include <gl.h>
#include <SDL.h>
#include "config.hpp"
#include "window.hpp"
#include "surface.hpp"
#include "glcontext.hpp"
#include "GL/gltexture.hpp"
using SDL::Window;
using SDL::GLContext;
using SDL::Surface;
using GL::Texture;

const char *bitmapName = "ok.bmp";

const int ERR_SDL_INIT = -1;

const GLchar vertexShaderSrc[] =
"#version 330 core\n"
"layout (location = 0) in vec3 v_pos;\n"
"layout (location = 1) in vec2 v_texcoord;\n"
"\n"
"out vec2 vf_texcoord;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(v_pos, 1.0f);\n"
"	vf_texcoord = v_texcoord;\n"
"}\n";

const GLchar fragShaderSrc[] =
"#version 330 core\n"
"in vec2 vf_texcoord;\n"
"\n"
"out vec4 color;\n"
"\n"
"uniform sampler2D tex;\n"
"\n"
"void main()\n"
"{\n"
"	color = texture(tex, vf_texcoord);\n"
"}\n";

const GLfloat vertdata[] = {
	// position		// texture coordinates
	 0.75f,  0.75f, 0.0f,	15.0f, 15.0f,
	 0.75f, -0.75f, 0.0f,	15.0f, 0.0f,
	-0.75f, -0.75f, 0.0f,	0.0f,  0.0f,
	-0.75f,  0.75f, 0.0f,	0.0f,  15.0f
};
constexpr GLsizei VERT_STRIDE = 5 * sizeof(GLfloat);

const GLuint indices[] = {
	0, 1, 3,
	1, 2, 3
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

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		SDL_Log("shader compilation failed. Shader type: 0x%x\n", type);

		constexpr std::size_t LOG_SIZE{256};
		std::unique_ptr<GLchar[]> log(new GLchar[LOG_SIZE]);
		glGetShaderInfoLog(shaderID, LOG_SIZE - 1, NULL, log.get());
		log[LOG_SIZE] = '\0'; // just in case

		SDL_Log("GL log: %s\n", log.get());
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
void setupGLObjects(GLuint &vao, GLuint &vbo, GLuint &ebo)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertdata), vertdata, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// attribute 0: position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERT_STRIDE,
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	// attribute 1: texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERT_STRIDE,
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void setupTexture(Texture &texture, const Surface &surface)
{
	texture.bind(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 
	Texture::image2D(GL_TEXTURE_2D, surface, GL_RGB, GL_UNSIGNED_BYTE);

	glGenerateMipmap(GL_TEXTURE_2D);

	texture.unbind(GL_TEXTURE_2D);
}

void gameLoop()
{
	Window window("test", Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOW_OPENGL);
	Surface surface{Surface::fromBitmap(bitmapName)};

	setContextFlags();
	GLContext context = window.makeGLContext();

	if(!initglew()) { return; }

	GLuint shaderProg = shaderProgSetup(vertexShaderSrc, fragShaderSrc);
	GLuint vao, vbo, ebo;
	setupGLObjects(vao, vbo, ebo);

	Texture texture(1);
	setupTexture(texture, surface);

	if(SDL_GL_SetSwapInterval(1) < 0) {
		SDL_Log("vsync not supported");
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
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture.bind(GL_TEXTURE_2D);
		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		texture.unbind(GL_TEXTURE_2D);

		window.swapWindow();
	}
	glDeleteProgram(shaderProg);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
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
