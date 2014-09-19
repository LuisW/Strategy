#pragma once

#include <vector>
#include "System.h"
#include "Camera.h"
#include "CreatureManager.h"
#include "CollisionSystem.h"

class CreatureAISystem : public System
{
private:
	EntityManager& entityManager;
	CreatureManager& creatureManager;
	CollisionSystem& collisionSystem;
	std::vector<EntityID> entities;

public:
	CreatureAISystem(EntityManager& _entityManager, CreatureManager& _creatureManager, CollisionSystem& _collisionSystem);

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);

	void Tick(Camera& player, double deltaT);
};