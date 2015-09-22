#pragma once

#include "ei/glm/glm.hpp"

struct LineSegment
{
	glm::vec3 m_p1;
	glm::vec3 m_p2;

	LineSegment(const glm::vec3& p1, const glm::vec3& p2) : m_p1(p1), m_p2(p2)
	{}
};

class Ray
{
private:
	glm::vec3 m_start;
	glm::vec3 m_dir;

public:
	Ray(glm::vec3 start, glm::vec3 dir) : m_start(start), m_dir(dir)
	{
	}

	inline const glm::vec3& getStart() const
	{
		return m_start;
	}

	inline const glm::vec3& getDir() const
	{
		return m_dir;
	}

	inline void setA(glm::vec3 start)
	{
		m_start = start;
	}
	
	inline void setDir(glm::vec3 dir)
	{
		m_dir = dir;
	}

	inline glm::vec3 getPointinRay(float t) const
	{
		return m_start + m_dir * t;
	}
};