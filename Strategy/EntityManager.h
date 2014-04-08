#pragma once

#include "Entity.h"

enum EntityBaseType {ET_Player};

class EntityManager
{
private:
	struct EntityListElem
	{
		Entity entity;
		bool used;
	};

	std::vector<EntityListElem> entities;

public:
	EntityManager()
	{
	}

	EntityID newEntity();
	void deleteEntity(EntityID entity);
	EntityID newPlayer();
	Entity& getEntity(EntityID entity);
	~EntityManager();
};