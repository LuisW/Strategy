#pragma once

#include "System.h"
#include <vector>
#include "ei/glm/glm.hpp"
#include "ei/GL/glew.h"
#include "EntityManager.h"

class RenderSystem : public System
{
private:
	EntityManager& entityManager;
	std::vector<EntityID> entities;

public:
	RenderSystem(EntityManager& _entityManager) : entityManager(_entityManager)
	{}

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);
	void Tick(const Camera& cam, GLuint shader) const;
};