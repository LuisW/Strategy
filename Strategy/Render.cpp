#include "Render.h"
#include "ei\GL\glew.h"
#include "ei/SDL/SDL_video.h"
#include "ei/SDL/SDL.h"
#include <iostream>
#include "RenderComponent.h"

void Render::RenderFrame()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_rData.getRenderSystem().Tick(*m_rData.getActiveCam());

	glm::vec3 cam = m_rData.getActiveCam()->getPos();

	//data.getTerrain().Render(data.getActiveCam()->getViewProjection(), cam);

	SDL_GL_SwapBuffers();
}