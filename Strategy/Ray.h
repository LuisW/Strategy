#pragma once

#include "ei/glm/glm.hpp"

struct LineSegment
{
	glm::vec3 p1;
	glm::vec3 p2;

	LineSegment(const glm::vec3& _p1, const glm::vec3& _p2) : p1(_p1), p2(_p2)
	{}
};

class Ray
{
private:
	glm::vec3 a;
	glm::vec3 d;

public:
	Ray(glm::vec3 _a, glm::vec3 _d) : a(_a), d(_d)
	{
	}

	inline const glm::vec3& getA() const
	{
		return a;
	}

	inline const glm::vec3& getDir() const
	{
		return d;
	}

	inline void setA(glm::vec3 _a)
	{
		a = _a;
	}
	
	inline void setDir(glm::vec3 _d)
	{
		d = _d;
	}

	inline glm::vec3 getPointinRay(float t) const
	{
		return a + d * t;
	}
};