#pragma once

#include "Component.h"
#include "ei/glm/glm.hpp"

class VelocityComponent : public Component
{
private:
	glm::vec3 m_velocity;

public:

	VelocityComponent(EntityID owner) : Component(CT_VELOCITY, owner, "")
	{
	}

	VelocityComponent(EntityID owner, glm::vec3 velocity) : Component(CT_VELOCITY, owner, ""), m_velocity(velocity)
	{
	}

	inline const glm::vec3& getVelocity()
	{
		return m_velocity;
	}

	inline void setVelocity(const glm::vec3& _velocity)
	{
		m_velocity = _velocity;
	}

	inline void operator+=(const glm::vec3& delta)
	{
		m_velocity += delta;
	}

	inline void setY(float y)
	{
		m_velocity.y = y;
	}

	inline void setX(float x)
	{
		m_velocity.x = x;
	}

	inline void setZ(float z)
	{
		m_velocity.z = z;
	}

	static ComponentType getComponentType()
	{
		return CT_VELOCITY;
	}
};