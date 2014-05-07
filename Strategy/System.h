#pragma once

class EntityManager;

#include "Entity.h"

class System
{
protected:
	EntityManager& entityManager;
public:
	System(EntityManager& _entityManager) : entityManager(_entityManager)
	{}
	 
	virtual void onEntityChanged(EntityID entity, ComponentType type, bool added) = 0;
	virtual void onEntityRemoved(EntityID entity) = 0;
};