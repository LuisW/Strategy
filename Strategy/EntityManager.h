#pragma once

#include "Entity.h"

enum EntityBaseType {ET_Triangle, ET_Rectangle};

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
	{}

	void RectangleRecipe();
	void TriangleRecipe();
};