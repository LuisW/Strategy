#pragma once

#include "ei/glm/glm.hpp"
#include "Component.h"
#include "Mesh.h"

class RenderComponent : public Component
{
private:
	MeshAsset_const mesh;
	glm::vec3 color;

public:
	RenderComponent(MeshAsset_const _mesh, EntityID _owner, const std::string& _name) : mesh(_mesh), color(0.6f), Component(CT_Render, _owner, _name)
	{
	}

	MeshAsset_const getMesh()
	{
		return mesh;
	}

	glm::vec3 getColor()
	{
		return color;
	}

	void setColor(glm::vec3& _color)
	{
		color = _color;
	}

	static ComponentType getComponentType()
	{
		return CT_Render;
	}
};