#pragma once

#include "ei/glm/glm.hpp"

class AABoundingBox
{
private:
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 center;
	glm::vec3 halfDiagonal;

public:
	AABoundingBox(glm::vec3 _min, glm::vec3 _max) : min(_min), max(_max), center((min + max) * 0.5f), halfDiagonal((max - min) * 0.5f)
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

	const glm::vec3& getCenter() const
	{
		return center;
	}

	const glm::vec3& getHalfDiagonal() const
	{
		return halfDiagonal;
	}

	void setMin(const glm::vec3& _min)
	{
		min = _min;
		center = (min + max) * 0.5f;
		halfDiagonal = (max - min) * 0.5f;
	}

	void setMax(const glm::vec3& _max)
	{
		max = _max;
		center = (min + max) * 0.5f;
		halfDiagonal = (max - min) * 0.5f;
	}
};