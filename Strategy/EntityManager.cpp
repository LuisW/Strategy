#include "EntityManager.h"

#include "CameraComponent.h"

EntityID EntityManager::newEntity()
{
	for (int n = 0; n < entities.size(); n++)
	{
		if (!entities[n].used)
		{
			entities[n].used = true;
			return n;
		}
	}

	EntityListElem newElem;
	newElem.entity = Entity(entities.size());
	newElem.used = true;
	entities.push_back(newElem);
	return newElem.entity.getID();
}

void EntityManager::deleteEntity(EntityID entity)
{
	entities[entity].entity.RemoveAllComponents();
	entities[entity].used = false;
}

Entity& EntityManager::getEntity(EntityID entity)
{
	return entities[entity].entity;
}

EntityID EntityManager::newPlayer()
{
	EntityID ent = newEntity();
}
