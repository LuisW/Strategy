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
			m_rData.m_keyboardState[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			m_rData.m_keyboardState[event.key.keysym.sym] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_rData.m_mouseBtnState[event.button.button] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			m_rData.m_mouseBtnState[event.button.button] = false;
			break;
		case SDL_QUIT:
			return false;
			break;
		}
	}

	if (m_rData.getKeyboardState(SDLK_ESCAPE))
		return false;

	if (m_rData.getKeyboardState(SDLK_l))
	{
		glPolygonMode(GL_FRONT, GL_LINE);
	}

	if (m_rData.getKeyboardState(SDLK_f))
	{
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	if (m_rData.getKeyboardState(SDLK_b))
	{
		__debugbreak();
	}

	m_rData.m_collisionSystem.PrepareFrame();

	if (m_rData.m_pActiveCamera != NULL)
	{
		m_rData.m_playerControl.Tick(m_rData.getPlayer(), deltaT);
		//data.cullingSystem.Tick(*data.getActiveCam());
	}

	return true;
}