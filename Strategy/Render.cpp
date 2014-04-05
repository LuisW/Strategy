#include "Render.h"
#include "ei\GL\glew.h"
#include "ei/SDL/SDL_video.h"
#include "ei/SDL/SDL.h"
#include <iostream>

void Render::RenderFrame()
{
	if (data.getMouseBtnState(SDL_BUTTON_X2))
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	else
		glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();
}