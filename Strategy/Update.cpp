#include "Update.h"
#include "ei/SDL/SDL.h"

bool Update::UpdateTick(double deltaT)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			data.KeyboardState[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			data.KeyboardState[event.key.keysym.sym] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			data.MouseBtnState[event.button.button] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			data.MouseBtnState[event.button.button] = false;
			break;
		case SDL_QUIT:
			return false;
			break;
		}
	}

	if (data.getKeyboardState(SDLK_ESCAPE))
		return false;

	if (data.getKeyboardState(SDLK_l))
	{
		glPolygonMode(GL_FRONT, GL_LINE);
	}

	if (data.getKeyboardState(SDLK_f))
	{
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	if (data.getKeyboardState(SDLK_b))
	{
		__debugbreak();
	}

	data.collisionSystem.PrepareFrame();

	if (data.activeCamera != NULL)
	{
		data.playerControl.Tick(data.getPlayer(), deltaT);
	}

	data.statSystem.Tick(deltaT);

	//data.cullingSystem.Tick(*(data.activeCamera));
	
	data.creatureAI.Tick(*(data.activeCamera), deltaT, data);
	data.projectileSystem.Tick(deltaT);

	return true;
}