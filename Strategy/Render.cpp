#include "Render.h"
#include "ei\GL\glew.h"
#include "ei/SDL/SDL_video.h"
#include "ei/SDL/SDL.h"
#include <iostream>
#include "RenderComponent.h"
#include "ei/glm/gtc/type_ptr.hpp"

void Render::RenderFrame()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glBindAttribLocation(data.getShader().get().getShader(), 0, "in_Position");
	glBindAttribLocation(data.getShader().get().getShader(), 1, "in_Normal");

	glLinkProgram(data.getShader().get().getShader());

	glUseProgram(data.getShader().get().getShader());

	GLbyte sp_WVP = glGetUniformLocation(data.getShader().get().getShader(), "WVP");
	glUniformMatrix4fv(sp_WVP, 1, GL_FALSE, glm::value_ptr(data.getActiveCam()->getViewProjection()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 48, NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 48, (GLvoid*)12);

	data.getRenderSystem().Tick();

	SDL_GL_SwapBuffers();
}