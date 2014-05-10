#pragma once

#include "Component.h"
#include "ei/glm/glm.hpp"
#include <vector>

class SceneGraphComponent : public Component
{
private:
	glm::mat4 parentMat;
	std::vector<EntityID> children;
	EntityID parent;


public:
	SceneGraphComponent(EntityID owner) : Component(CT_SceneGraph, owner, ""), parentMat()
	{
	} 

	inline std::vector<EntityID>& getChildren()
	{
		return children;
	}

	inline const glm::mat4& getParentMat() const
	{
		return parentMat;
	}

	inline EntityID getParent()	const
	{
		return parent;
	}

	inline void setParent(EntityID _parent)
	{
		parent = _parent;
	}


};