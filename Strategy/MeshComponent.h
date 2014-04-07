#pragma once

#include "ei/glm/glm.hpp"
#include "Component.h"

class MeshComponent : Component
{
private:
	glm::vec3 color;
	glm::vec3* verts;
public:
	MeshComponent(unsigned int size, const glm::vec3* _verts, glm::vec3 _color, EntityID owner, const std::string& name) : Component(CT_Mesh, owner, name), color(_color)
	{
		verts = new glm::vec3[size];
		memcpy(verts, _verts, sizeof(glm::vec3) * size);
	}

	inline void setVertex(unsigned int index, glm::vec3 value)
	{
		verts[index] = value;
	}

	inline glm::vec3 getVertex(unsigned int index)
	{
		return verts[index];
	}

	inline void setColor(glm::vec3 _color)
	{
		color = _color;
	}

	inline glm::vec3 getColor()
	{
		return color;
	}

	~MeshComponent()
	{
		delete[] verts;
	}
};