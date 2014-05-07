#pragma once

#include "System.h"
#include <vector>

class RenderSystem : public System
{
private:
	std::vector<EntityID> entities;

public:
	RenderSystem(EntityManager& _entityManager) : System(_entityManager)
	{}

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);
	void Tick() const;
};