#pragma once

#include "EntityManager.h"

class ICollisionFilter
{
private:
public:
	virtual bool Filter(EntityID ent, EntityManager& entityManager) = 0;
	virtual ~ICollisionFilter(){};
};

class DefaultCollisionFilter : public ICollisionFilter
{
private:
public:
	bool Filter(EntityID ent, EntityManager& entityManager)
	{
		return true;
	}
};

template<ComponentType type>
class ComponentCollisionFilter : public ICollisionFilter
{
private:
public:
	bool Filter(EntityID ent, EntityManager& entityManager)
	{
		return entityManager.entityHasComponent(ent, type);
	}
};
