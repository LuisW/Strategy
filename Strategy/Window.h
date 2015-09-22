#pragma once

#include "ei\SDL\SDL.h"
#include "ei\SDL\SDL_video.h"
#include "ei\GL\glew.h"
#include <string>
#undef main

class Window
{
private:
	SDL_Surface* m_pSurface;
	unsigned int m_width;
	unsigned int m_height;
	bool m_fullscreen;
	std::string m_title;

	void initGL();
	void initSDL();
public:
	Window(unsigned int width, unsigned int height, std::string title, bool fullscreen);
	unsigned int getWidth();
	unsigned int getHeight();
	std::string getTile();
	~Window();
};