#pragma once

#include "Component.h"
#include "ei/glm/glm.hpp"

class VelocityComponent : public Component
{
private:
	glm::vec3 velocity;

public:

	VelocityComponent(EntityID owner) : Component(CT_Velocity, owner, "")
	{
	}

	VelocityComponent(EntityID owner, glm::vec3 _velocity) : Component(CT_Velocity, owner, ""), velocity(_velocity)
	{
	}

	inline const glm::vec3& getVelocity()
	{
		return velocity;
	}

	inline void setVelocity(const glm::vec3& _velocity)
	{
		velocity = _velocity;
	}

	inline void operator+=(const glm::vec3& delta)
	{
		velocity += delta;
	}

	inline void setY(float y)
	{
		velocity.y = y;
	}

	static ComponentType getComponentType()
	{
		return CT_Velocity;
	}
};