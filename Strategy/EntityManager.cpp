#include "EntityManager.h"

#include "CameraComponent.h"

Entity& EntityManager::newEntity()
{
	for (int n = 0; n < entities.size(); n++)
	{
		if (!entities[n].used)
		{
			entities[n].used = true;
			return *entities[n].entity;
		}
	}

	EntityListElem newElem;
	newElem.entity = new Entity(entities.size());
	newElem.used = true;
	entities.push_back(newElem);
	return *entities.back().entity;
}

void EntityManager::deleteEntity(EntityID entity)
{
	entities[entity].entity->RemoveAllComponents();
	entities[entity].used = false;
}

Entity& EntityManager::getEntity(EntityID entity)
{
	return *entities[entity].entity;
}

const Entity& EntityManager::getEntityConst(EntityID entity) const
{
	return *entities[entity].entity;
}

Entity& EntityManager::newPlayer()
{
	Entity& ent = newEntity();
	//ent.AddComponent(new CameraComponent());
	return ent;
}

EntityManager::~EntityManager()
{
	for (int n = 0; n < entities.size(); n++)
	{
		delete entities[n].entity;
	}
}