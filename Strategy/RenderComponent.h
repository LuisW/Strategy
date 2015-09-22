#pragma once

#include "ei/glm/glm.hpp"
#include "Component.h"
#include "Mesh.h"
#include "MaterialTemplateLib.h"
#include "VertexFormat.h"
#include "AssetManager.h"

enum EngineUniform { EUNI_WVP, EUNI_NMAT, EUNI_WV, EUNI_V, EUNI_UNUSED };
static const char* EngUniformStrings[] = { "eng_WVP", "eng_NMat", "eng_WV", "eng_V" };

class RenderComponent : public Component
{
private:
	MeshAsset_const m_mesh;
	MaterialAsset_const m_mat;
	MeshToMaterialMap m_formatBind;
	MaterialObjectData m_objData;
	UniformID m_engVars[EUNI_UNUSED];

public:
	RenderComponent(MeshAsset_const mesh, EntityID owner, const std::string& name) : m_mesh(mesh),
		m_mat(AssetManager::getAsset<Material>(MaterialKey("ConstColor","Materials.mat"))),
		m_formatBind(LinkMeshToMaterial(m_mesh.get(), m_mat.get().getFormat())), 
		m_objData(m_mat.get().newObjectParams()), Component(CT_RENDER, owner, name)
	{
		for (unsigned int n = 0; n < EUNI_UNUSED; n++)
		{
			m_engVars[n] = m_objData.checkUniform(EngUniformStrings[n]);
		}
	}

	inline UniformID getEngineUniformID(EngineUniform uniform)
	{
		return m_engVars[uniform];
	}

	MeshAsset_const getMesh()
	{
		return m_mesh;
	}

	MaterialAsset_const getMaterial() const
	{
		return m_mat;
	}

	const MeshToMaterialMap& getFormatBind() const
	{
		return m_formatBind;
	}

	MaterialObjectData& getObjectData()
	{
		return m_objData;
	}

	static ComponentType getComponentType()
	{
		return CT_RENDER;
	}

	~RenderComponent()
	{
	}
};