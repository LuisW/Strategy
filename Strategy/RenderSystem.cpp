#include "RenderSystem.h"
#include "EntityManager.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ei/glm/gtc/type_ptr.hpp"

void RenderSystem::onEntityChanged(EntityID entity, ComponentType type, bool added)
{
	if (type != CT_RENDER)
	{
		return;
	}

	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (m_entities[n] == entity)
		{
			if (!added)
			{
				m_entities.erase(m_entities.begin() + n);
			}
			return;
		}
	}

	m_entities.push_back(entity);
}

void RenderSystem::onEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (m_entities[n] == entity)
		{
			m_entities.erase(m_entities.begin() + n);
		}
	}
}

void RenderSystem::Tick(const Camera& cam) const
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		RenderComponent& Comp = m_rEntityManager.entityGetComponent<RenderComponent>(m_entities[n]);

		const Material& mat = Comp.getMaterial().get();
		MaterialObjectData& objData = Comp.getObjectData();
		mat.BindShader();
		mat.BindMaterialParams();

		UniformID loc_nmat = Comp.getEngineUniformID(EUNI_NMAT);
		UniformID loc_wvp = Comp.getEngineUniformID(EUNI_WVP);
		UniformID loc_wv = Comp.getEngineUniformID(EUNI_WV);
		UniformID loc_v = Comp.getEngineUniformID(EUNI_V);

		if (m_rEntityManager.entityHasComponent(m_entities[n], CT_TRANSFORM))
		{
			TransformComponent& trans = m_rEntityManager.entityGetComponent<TransformComponent>(m_entities[n]);
			
			glm::mat4 WV = cam.getView() * trans.getMat();

			if (loc_nmat >= 0)
			{
				glm::mat3 nmat = glm::transpose(glm::inverse(glm::mat3(WV)));
				objData.setUniform(loc_nmat, nmat);
			}

			objData.setUniform(loc_wvp, cam.getViewProjection() * trans.getMat());

			if (loc_wv >= 0)
			{
				objData.setUniform(loc_wv, WV);
			}

			objData.setUniform(loc_v, cam.getView());
		}
		else
		{
			objData.setUniform(loc_nmat, glm::mat3());
			objData.setUniform(loc_wvp, cam.getViewProjection());
			objData.setUniform(loc_wv, cam.getView());
			objData.setUniform(loc_v, cam.getView());

		}

		mat.BindObjectParams(objData);

		const Mesh& mesh = Comp.getMesh().get();

		Comp.getFormatBind().Bind();

		glDrawElements(GL_TRIANGLES, mesh.getIndlen(), GL_UNSIGNED_INT, NULL);
	}

	glBindVertexArray(0);
}