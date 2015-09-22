#pragma once

#include "ei/glm/glm.hpp"
#include "BoundingBox.h"

enum FrustumPlane {FP_TOP, FP_BOTTOM, FP_LEFT, FP_RIGHT, FP_NEAR, FP_FAR};

const char PlaneMasks[6] = { 1, 2, 4, 8, 16, 32 };

class Frustum
{
private:

	struct Plane
	{
		glm::vec4 m_plane;

		IntersectionType IntersectAABB(const AABB& box)
		{
			glm::vec3 h = box.getHalfDiagonal();
			glm::vec3 c = box.getCenter();

			float e = h.x * abs(m_plane.x) + h.y * abs(m_plane.y) + h.z * abs(m_plane.z);
			float s = c.x * m_plane.x + c.y * m_plane.y + c.z * m_plane.z + m_plane.w;

			if (s - e > 0) return IT_Outside;
			if (s + e < 0) return IT_Inside;
			return IT_Intersect;
		}

		IntersectionType IntersectAABB(const glm::vec3& c, const glm::vec3& h)
		{
			float e = h.x * abs(m_plane.x) + h.y * abs(m_plane.y) + h.z * abs(m_plane.z);
			float s = c.x * m_plane.x + c.y * m_plane.y + c.z * m_plane.z + m_plane.w;

			if (s - e > 0) return IT_Outside;
			if (s + e < 0) return IT_Inside;
			return IT_Intersect;
		}

		void Normalize()
		{
			float len = glm::sqrt(m_plane.x * m_plane.x + m_plane.y * m_plane.y + m_plane.z * m_plane.z);
			m_plane /= len;
		}
	};

	Plane m_planes[6];

public:
	Frustum(glm::mat4 VP)
	{
		VP = glm::transpose(VP);
		m_planes[FP_LEFT  ].m_plane = -(VP[3] + VP[0]);
		m_planes[FP_RIGHT ].m_plane = -(VP[3] - VP[0]);
		m_planes[FP_BOTTOM].m_plane = -(VP[3] + VP[1]);
		m_planes[FP_TOP   ].m_plane = -(VP[3] - VP[1]);
		m_planes[FP_NEAR  ].m_plane = -(VP[3] + VP[2]);
		m_planes[FP_FAR   ].m_plane = -(VP[3] - VP[2]);

		for (int n = 0; n < 6; n++)
		{
			m_planes[n].Normalize();
		}
	}					    

	inline const glm::vec4& getPlane(FrustumPlane plane)
	{
		return m_planes[plane].m_plane;
	}

	IntersectionType IntersectAABB(const AABB& box)
	{
		bool intersect = false;

		for (int k = 0; k < 6; k++)
		{
			IntersectionType it = m_planes[k].IntersectAABB(box);
			if (it == IT_Outside) return IT_Outside;
			else if (it == IT_Intersect) intersect = true;
		}

		if (intersect) return IT_Intersect;
		return IT_Inside;
	}

	IntersectionType IntersectAABB(const AABB& box, char& parentIn)
	{
		bool intersect = false;

		IntersectionType it;

		for (unsigned int k = 0; k < 6; ++k)
		{
			if ((1 << k) & parentIn)
			{
				continue;
			}

			it = m_planes[k].IntersectAABB(box);
			if (it == IT_Outside)
			{
				return IT_Outside; 
			}
			else if (it == IT_Intersect) intersect = true;
			else parentIn |= (1 << k);
		}

		if (intersect) return IT_Intersect;
		return IT_Inside;
	}

	IntersectionType IntersectAABB(const glm::vec3& c, const glm::vec3& h, char& parentIn)
	{
		bool intersect = false;

		IntersectionType it;

		for (unsigned int k = 0; k < 6; ++k)
		{
			if ((1 << k) & parentIn)
			{
				continue;
			}

			it = m_planes[k].IntersectAABB(c, h);
			if (it == IT_Outside)
			{
				return IT_Outside;
			}
			else if (it == IT_Intersect) intersect = true;
			else parentIn |= (1 << k);
		}

		if (intersect) return IT_Intersect;
		return IT_Inside;
	}
};