#pragma once

#include "Entity.h"

enum EntityBaseType {ET_Player};

class EntityManager
{
private:
	struct EntityListElem
	{
		Entity* entity;
		bool used;
	};

	std::vector<EntityListElem> entities;

	EntityManager(const EntityManager& other)
	{}

	const EntityManager& operator=(const EntityManager& rhs)
	{}

public:
	EntityManager()
	{
	}

	Entity& newEntity();
	void deleteEntity(EntityID entity);
	Entity& getEntity(EntityID entity);
	const Entity& getEntityConst(EntityID entity) const;
	~EntityManager();

	Entity& newPlayer();
};