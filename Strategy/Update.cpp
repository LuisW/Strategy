#include "Update.h"
#include "ei/SDL/SDL.h"

bool Update::UpdateTick()
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

	return true;
}