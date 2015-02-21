#include "PlayerControlSystem.h"
#include "ei/SDL/SDL.h"
#include "VelocityComponent.h"
#include "GameData.h"
#include "GunAssetGenerator.h"
#include <iostream>

void PlayerControlSystem::Tick(EntityID player, double deltaT)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	mx -= 512;
	my -= 384;

	Camera& activeCam = data.getEntityManager().entityGetComponent<CameraComponent>(player);
	Transform& trans = data.getEntityManager().entityGetComponent<TransformComponent>(player);
	StatsComponent& stats = data.getEntityManager().entityGetComponent<StatsComponent>(player);
	VelocityComponent& vel = data.getEntityManager().entityGetComponent<VelocityComponent>(player);
		
	activeCam.setPos(trans.getPos());
	activeCam.AddYawPitch(0.1f * mx, 0.1f * my);

	float speed = stats.derived.movementspeed;

	if (data.getKeyboardState(SDLK_LSHIFT))
	{
		speed *= 3.0f;
	}

	SDL_WarpMouse(512, 384);

	glm::vec3 walkfront = activeCam.getFront();
	walkfront.y = 0.0f;
	walkfront = glm::normalize(walkfront);

	glm::vec3 walkright = activeCam.getRight();
	walkright.y = 0.0f;
	walkright = glm::normalize(walkright);

	vel.setX(0.0f);
	vel.setZ(0.0f);

	if (data.getKeyboardState(SDLK_w))
	{
		vel += walkfront * speed;
	}

	if (data.getKeyboardState(SDLK_a))
	{
		vel += walkright * -speed;
	}

	if (data.getKeyboardState(SDLK_s))
	{
		vel += walkfront * -speed;
	}

	if (data.getKeyboardState(SDLK_d))
	{
		vel += walkright * speed;
	}

	if (data.getMouseBtnState(SDL_BUTTON_LEFT) && fullAuto + 100 < SDL_GetTicks())
	{
		std::vector<EntityID> ents;
		data.getCollisionSystem().Collide(Ray(activeCam.getPos(), activeCam.getFront()), true, ents, ComponentCollisionFilter<CT_Vitals>());

		activeCam.AddYawPitch(randFltWeighted1(SDL_GetTicks(), 0.0f, 2.0f), -1.0f);

		for (unsigned int n = 0; n < ents.size(); n++)
		{
			VitalsComponent& vitals = data.getEntityManager().entityGetComponent<VitalsComponent>(ents[n]);
			vitals.healthPercent -= 500.0f / vitals.maxHealth;

			if (vitals.healthPercent <= 0.0f)
				data.getEntityManager().deleteEntity(ents[n]);
		}

		fullAuto = SDL_GetTicks();
	}

	float h = data.getCollisionSystem().TerrainHeight(glm::vec2(activeCam.getPos().x, activeCam.getPos().z));

	if (data.getKeyboardState(SDLK_SPACE))
	{
		if (activeCam.getPos().y < (h + 1.8001f))
		{
			vel += glm::vec3(0.0f, 4.4294f, 0.0f);
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

	if (data.getKeyboardState(SDLK_t))
	{
		if (tUp)
		{
			unsigned int seed = SDL_GetTicks() % 1000;
			std::cout << "ProceduralAssaultRifle:" << seed << std::endl;
			data.getEntityManager().newStaticMesh(GunAssetGenerator::GenerateGun(seed), trans);
			tUp = false;
		}
	}
	else
	{
		tUp = true;
	}

	glm::quat trot = activeCam.getRot();
	trot.x = 0;
	trot.z = 0;

	trans.setPos(activeCam.getPos());
	trans.setRot(trot / (trot.y*trot.y + trot.w*trot.w));
}