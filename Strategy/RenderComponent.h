#pragma once

#include "ei/glm/glm.hpp"
#include "Component.h"
#include "Mesh.h"

class RenderComponent : public Component
{
private:
	MeshAsset_const mesh;
public:
	RenderComponent(MeshAsset_const _mesh, EntityID _owner, const std::string& _name) : mesh(_mesh), Component(CT_Render, _owner, _name)
	{
	}

	MeshAsset_const getMesh()
	{
		return mesh;
	}

	static ComponentType getComponentType()
	{
		return CT_Render;
	}
};