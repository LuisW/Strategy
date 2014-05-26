#pragma once

#include "ei/glm/glm.hpp"

class AABoundingBox
{
private:
	glm::vec3 min;
	glm::vec3 max;

public:
	AABoundingBox(glm::vec3 _min, glm::vec3 _max) : min(_min), max(_max)
	{
	}

	const glm::vec3& getMin() const
	{
		return min;
	}

	const glm::vec3& getMax() const
	{
		return max;
	}

	inline glm::vec3 getCenter() const
	{
		return 0.5f * (min + max);
	}

	inline glm::vec3 getHalfDiagonal() const
	{
		return 0.5f * (max - min);
	}

	void setMin(const glm::vec3& _min)
	{
		min = _min;
	}

	void setMax(const glm::vec3& _max)
	{
		max = _max;
	}

	inline float SqDistanceToPointXZ(const glm::vec3& p)
	{
		glm::vec3 center = getCenter();
		float dx = glm::max(glm::abs(p.x - center.x) - (center.x - min.x), 0.0f);
		float dz = glm::max(glm::abs(p.z - center.z) - (center.z - min.z), 0.0f);

		return dx*dx + dz*dz;
	}
};