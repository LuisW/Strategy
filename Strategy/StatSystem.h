#pragma once

#include "System.h"

class StatSystem : public System
{
private:
	std::vector<EntityID> entities;
	EntityManager& entityManager;

public:
	StatSystem(EntityManager& _entityManager);

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);

	void Tick(double deltaT);
};