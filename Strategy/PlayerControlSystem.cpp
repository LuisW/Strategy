#include "PlayerControlSystem.h"
#include "ei/SDL/SDL.h"

void PlayerControlSystem::Tick(Camera& activeCam)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	mx -= 512;
	my -= 384;

	activeCam.Rotate(glm::angleAxis(my * 0.1f, activeCam.getRight()) * glm::angleAxis(mx * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f)));

	SDL_WarpMouse(512, 384);

	if (data.getKeyboardState(SDLK_w))
	{
		activeCam.Move(1.0f);
	}

	if (data.getKeyboardState(SDLK_a))
	{
		activeCam.Strafe(-1.0f);
	}

	if (data.getKeyboardState(SDLK_s))
	{
		activeCam.Move(-1.0f);
	}

	if (data.getKeyboardState(SDLK_d))
	{
		activeCam.Strafe(1.0f);
	}
}