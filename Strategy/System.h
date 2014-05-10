#pragma once

class EntityManager;

#include "Entity.h"

class System
{
public:
	virtual void onEntityChanged(EntityID entity, ComponentType type, bool added) = 0;
	virtual void onEntityRemoved(EntityID entity) = 0;
};