#pragma once

#include "System.h"
#include <vector>
#include "ei/glm/glm.hpp"
#include "ei/GL/glew.h"
#include "EntityManager.h"

class RenderSystem : public System
{
private:
	EntityManager&			m_rEntityManager;
	std::vector<EntityID>	m_entities;

public:
	RenderSystem(EntityManager& entityManager) : m_rEntityManager(entityManager)
	{}

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);
	void Tick(const Camera& cam) const;
};