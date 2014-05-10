#pragma once

#include "System.h"
#include <vector>
#include "ei/glm/glm.hpp"
#include "ei/GL/glew.h"

class RenderSystem : public System
{
private:
	EntityManager& entities;
	std::vector<EntityID> entities;

public:
	RenderSystem(EntityManager& _entityManager)
	{}

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);
	void Tick(const glm::mat4& VP, GLbyte sp_WVP) const;
};