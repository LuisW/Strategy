#pragma once

#include "Component.h"
#include "Transform.h"

class TransformComponent : public Component, public Transform
{
private:
public:
	TransformComponent(EntityID owner, const std::string& name, const Transform& transform) : Component(CT_TRANSFORM, owner, name) , Transform(transform)
	{}

	TransformComponent(EntityID owner, const std::string& name, const glm::vec3& Pos, const glm::quat& Rot, const glm::vec3& Scl) : Component(CT_TRANSFORM, owner, name), Transform(Pos, Rot, Scl)
	{}

	TransformComponent(EntityID owner, const std::string& name) : Component(CT_TRANSFORM, owner, name)
	{}

	static ComponentType getComponentType()
	{
		return CT_TRANSFORM;
	}
};