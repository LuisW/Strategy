#pragma once

#include "System.h"
#include "EntityManager.h"
#include <vector>
#include "CollisionSystem.h"
#include "BulletComponent.h"
#include "GroundWaveComponent.h"

class ProjectileSystem : public System
{
private:
	EntityManager& entityManager;
	CollisionSystem& collisionSystem;

	std::vector<EntityID> ents[PT_Unused];
	
	std::vector<CollDistData> collisions;

public:
	ProjectileSystem(EntityManager& _entityManager, CollisionSystem& _collisionSystem);

	void Tick(double deltaT);

	bool BulletTick(unsigned int n, double deltaT);
	bool GroundWaveTick(unsigned int n, double deltaT);

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);
};