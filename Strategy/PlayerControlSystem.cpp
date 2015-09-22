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

	Camera& activeCam = m_rData.getEntityManager().entityGetComponent<CameraComponent>(player);
	Transform& trans = m_rData.getEntityManager().entityGetComponent<TransformComponent>(player);
	VelocityComponent& vel = m_rData.getEntityManager().entityGetComponent<VelocityComponent>(player);
		
	activeCam.setPos(trans.getPos());
	activeCam.AddYawPitch(0.1f * mx, 0.1f * my);

	float speed = 3.0f;

	if (m_rData.getKeyboardState(SDLK_LSHIFT))
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

	if (m_rData.getKeyboardState(SDLK_w))
	{
		vel += walkfront * speed;
	}

	if (m_rData.getKeyboardState(SDLK_a))
	{
		vel += walkright * -speed;
	}

	if (m_rData.getKeyboardState(SDLK_s))
	{
		vel += walkfront * -speed;
	}

	if (m_rData.getKeyboardState(SDLK_d))
	{
		vel += walkright * speed;
	}

	float h = m_rData.getCollisionSystem().TerrainHeight(glm::vec2(activeCam.getPos().x, activeCam.getPos().z));

	h = -1.8f;

	if (m_rData.getKeyboardState(SDLK_SPACE))
	{
		if (activeCam.getPos().y < (h + 1.8001f))
		{
			vel += glm::vec3(0.0f, 4.4294f, 0.0f);
		}
	}

	glm::vec3 newPos = activeCam.getPos() + vel.getVelocity() * (float)deltaT;

	if (newPos.y < h + 1.8f - 1e-06f)
	{
		newPos.y = h + 1.8f;
		vel.setY(0.0f);
	}

	activeCam.setPos(newPos);

	vel += glm::vec3(0.0f, -9.81f  * deltaT, 0.0f);

	if (m_rData.getKeyboardState(SDLK_t))
	{
		if (m_tUp)
		{
			unsigned int seed = SDL_GetTicks() % 1000;
			std::cout << "ProceduralAssaultRifle:" << seed << std::endl;
			m_rData.getEntityManager().newStaticMesh(GunAssetGenerator::GenerateGun(seed), trans);
			m_tUp = false;
		}
	}
	else
	{
		m_tUp = true;
	}

	glm::quat trot = activeCam.getRot();
	trot.x = 0;
	trot.z = 0;

	trans.setPos(activeCam.getPos());
	trans.setRot(trot / (trot.y*trot.y + trot.w*trot.w));
}