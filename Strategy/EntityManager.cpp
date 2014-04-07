#include "EntityManager.h"

#include "MeshComponent.h"

void EntityManager::RectangleRecipe()
{
	entities.push_back(Entity(mkID()));
	entities.back().AddComponent(new MeshComponent(6,)
}