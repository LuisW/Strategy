#pragma once

#include "Entity.h"

enum EntityBaseType {ET_Player};

class EntityManager
{
private:
	std::vector<Entity> entities;
	EntityID idCounter;
	
	inline EntityID mkID()
	{
		return idCounter++;
	}

public:
	EntityManager() : idCounter(0)
	{
	}

	Entity& newPlayer();
};