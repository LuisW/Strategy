#pragma once

#include "System.h"
#include "EntityManager.h"
#include <vector>
#include "CollisionSystem.h"

class ProjectileSystem : public System
{
private:
	EntityManager& entityManager;
	CollisionSystem& collisionSystem;

	std::vector<EntityID> ents;
	
public:
	ProjectileSystem(EntityManager& _entityManager, CollisionSystem& _collisionSystem);

	void Tick(double deltaT);

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);
};