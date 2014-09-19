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
			}
			return;
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

void RenderSystem::Tick(const Camera& cam, GLuint shader) const
{
	GLuint sp_WVP = glGetUniformLocation(shader, "WVP");
	GLuint sp_NMat = glGetUniformLocation(shader, "NMat");
	GLuint sp_Color = glGetUniformLocation(shader, "Color");

	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (entityManager.entityHasComponent(entities[n], CT_Transform))
		{
			TransformComponent& trans = entityManager.entityGetComponent<TransformComponent>(entities[n]);
			glm::mat3 nmat = glm::transpose(glm::inverse(glm::mat3(trans.getMat())));
			glUniformMatrix3fv(sp_NMat, 1, GL_FALSE, glm::value_ptr(nmat));
			glUniformMatrix4fv(sp_WVP, 1, GL_FALSE, glm::value_ptr(cam.getViewProjection() * trans.getMat()));
		}
		else
		{
			glUniformMatrix3fv(sp_NMat, 1, GL_FALSE, glm::value_ptr(glm::mat3()));
			glUniformMatrix4fv(sp_WVP, 1, GL_FALSE, glm::value_ptr(cam.getViewProjection()));
		}

		RenderComponent& Comp = entityManager.entityGetComponent<RenderComponent>(entities[n]);

		glUniform3fv(sp_Color, 1, glm::value_ptr(Comp.getColor()));

		const Mesh& mesh = Comp.getMesh().get();

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertices());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, NULL);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (GLvoid*)12);
		glVertexAttribDivisor(1, 0);

		glDrawElements(GL_TRIANGLES, mesh.getIndlen(), GL_UNSIGNED_INT, NULL);
	}
}