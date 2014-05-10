#include "RenderSystem.h"
#include "EntityManager.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ei/glm/gtc/type_ptr.hpp"

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

void RenderSystem::Tick(const glm::mat4& VP, GLbyte sp_WVP) const
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (entityManager.entityHasComponent(entities[n], CT_Transform))
		{
			TransformComponent& trans = entityManager.entityGetComponent<TransformComponent>(entities[n]);
			glUniformMatrix4fv(sp_WVP, 1, GL_FALSE, glm::value_ptr(VP * trans.getMat()));
		}
		else
		{
			glUniformMatrix4fv(sp_WVP, 1, GL_FALSE, glm::value_ptr(VP));
		}

		RenderComponent& Comp = entityManager.entityGetComponent<RenderComponent>(entities[n]);

		const Mesh& mesh = Comp.getMesh().get();

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertices());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices());

		glDrawElements(GL_TRIANGLES, mesh.getIndlen(), GL_UNSIGNED_INT, NULL);
	}
}