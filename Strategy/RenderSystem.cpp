#include "RenderSystem.h"
#include "EntityManager.h"
#include "ei/GL/glew.h"
#include "RenderComponent.h"

void RenderSystem::onEntityChanged(EntityID entity, ComponentType type, bool added)
{
	if (type != CT_Render)
	{
		return;
	}

	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (entities[n] == entity)
		{
			if (!added)
			{
				entities.erase(entities.begin() + n);
				return;
			}
		}
	}

	entities.push_back(entity);
}

void RenderSystem::onEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (entities[n] == entity)
		{
			entities.erase(entities.begin() + n);
		}
	}
}

void RenderSystem::Tick() const
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		RenderComponent& Comp = entityManager.entityGetComponent<RenderComponent>(entities[n]);

		const Mesh& mesh = Comp.getMesh().get();

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertices());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices());

		glDrawElements(GL_TRIANGLES, mesh.getIndlen(), GL_UNSIGNED_INT, NULL);
	}
}