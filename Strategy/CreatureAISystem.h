#pragma once

#include <vector>
#include "System.h"
#include "Camera.h"
#include "CreatureManager.h"
#include "CollisionSystem.h"

class GameData;

class CreatureAISystem : public System
{
private:
	friend IAbility;

	EntityManager& entityManager;
	CreatureManager& creatureManager;
	CollisionSystem& collisionSystem;
	std::vector<EntityID> entities;

	void makeAIPlan(EntityID ent);
	bool predictHit(glm::vec3& hitPos, EntityID player, EntityID ent, const IAbility& spell);
	void setTargetTransformation(EntityID ent, const IAbility& spell, const glm::vec3& aimPos);
	int getMoveSkill(EntityID player, EntityID ent);

public:
	CreatureAISystem(EntityManager& _entityManager, CreatureManager& _creatureManager, CollisionSystem& _collisionSystem);

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);

	void Tick(Camera& player, double deltaT, GameData& data);
};