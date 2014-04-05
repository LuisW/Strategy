#pragma once

#include "ei\SDL\SDL.h"
#include "ei\SDL\SDL_video.h"
#include "ei\GL\glew.h"
#include <string>
#undef main

class Window
{
private:
	SDL_Surface* surface;
	unsigned int width;
	unsigned int height;
	bool fullscreen;
	std::string title;

	void initGL();
	void initSDL();
public:
	Window(unsigned int width, unsigned int height, std::string title, bool fullscreen);
	unsigned int getWidth();
	unsigned int getHeight();
	std::string getTile();
	~Window();
};