#include "PlayerControlSystem.h"
#include "ei/SDL/SDL.h"
#include "VelocityComponent.h"
#include "GameData.h"

void PlayerControlSystem::Tick(EntityID player, double deltaT)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	mx -= 512;
	my -= 384;

	Camera& activeCam = data.getEntityManager().entityGetComponent<CameraComponent>(player);
	Transform& trans = data.getEntityManager().entityGetComponent<TransformComponent>(player);
		
	activeCam.AddYawPitch(0.1f * mx, 0.1f * my);

	float speed = 5.0f * (float)deltaT;

	if (data.getKeyboardState(SDLK_LSHIFT))
	{
		speed *= 3.0f;
	}

	SDL_WarpMouse(512, 384);

	if (data.getKeyboardState(SDLK_w))
	{
		activeCam.Move(speed);
	}

	if (data.getKeyboardState(SDLK_a))
	{
		activeCam.Strafe(-speed);
	}

	if (data.getKeyboardState(SDLK_s))
	{
		activeCam.Move(-speed);
	}

	if (data.getKeyboardState(SDLK_d))
	{
		activeCam.Strafe(speed);
	}

	if (data.getMouseBtnState(SDL_BUTTON_LEFT))
	{
		if (rmbUp)
		{
			std::vector<EntityID> ents;
			data.getCollisionSystem().Collide(Ray(activeCam.getPos(), activeCam.getFront()), true, ents);

			for (unsigned int n = 0; n < ents.size(); n++)
			{
				data.getEntityManager().deleteEntity(ents[n]);
			}

			rmbUp = false;
		}
	}
	else
	{
		rmbUp = true;
	}

	VelocityComponent& vel = data.getEntityManager().entityGetComponent<VelocityComponent>(player);

	float h = data.getCollisionSystem().TerrainHeight(glm::vec2(activeCam.getPos().x, activeCam.getPos().z));

	if (data.getKeyboardState(SDLK_SPACE))
	{
		if (activeCam.getPos().y < (h + 1.8001f))
		{
			vel.setVelocity(glm::vec3(0.0f, 4.4294f, 0.0f));
		}
	}

	glm::vec3 newPos = activeCam.getPos() + vel.getVelocity() * (float)deltaT;

	if (newPos.y < h + 1.8f)
	{
		newPos.y = h + 1.8f;
		vel.setY(0.0f);
	}

	activeCam.setPos(newPos);

	vel += glm::vec3(0.0f, -9.81f  * deltaT, 0.0f);

	if (data.getKeyboardState(SDLK_r))
	{
		if (rUp)
		{
			data.getEntityManager().newCreature(data.getCreatureManager(), SDL_GetTicks() % 1000, newPos);
			rUp = false;
		}
	}
	else
	{
		rUp = true;
	}

	trans.setPos(activeCam.getPos() + activeCam.getFront() * 0.2f + activeCam.getRight() * 0.1f - activeCam.getUp() * 0.2f);
	trans.setRot(glm::inverse(activeCam.getRot()) * glm::angleAxis(90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
}