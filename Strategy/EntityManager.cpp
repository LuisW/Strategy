#include "EntityManager.h"

#include "CameraComponent.h"

Entity& EntityManager::newPlayer()
{
	entities.push_back(Entity(mkID()));
	entities.back().AddComponent(new CameraComponent(entities.back().getID(), "PlayerCam", 60.0f, 4.0f / 3.0f, 1.0f, 10000.0f));
	return entities.back();
}