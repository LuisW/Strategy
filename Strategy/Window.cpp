#include "Window.h"

void Window::initSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
}

void Window::initGL()
{
	SDL_WM_SetCaption(title.c_str(), NULL);

	Uint32 flags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;

	if (fullscreen)
	{
		flags |= SDL_FULLSCREEN;
	}

	surface = SDL_SetVideoMode(width, height, 32, flags);

	glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);
	glClearDepth(1.0f);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_FILL);

	glewInit();
}


Window::Window(unsigned int _width, unsigned int _height, std::string _title, bool _fullscreen) : width(_width), height(_height), title(_title), fullscreen(_fullscreen)
{
	initSDL();
	initGL();
}

unsigned int Window::getWidth()
{
	return width;
}

unsigned int Window::getHeight()
{
	return height;
}

std::string Window::getTile()
{
	return title;
}

Window::~Window()
{
	SDL_Quit();
}