#pragma once

#include "ei/glm/glm.hpp"
#include "BoundingBox.h"

enum FrustumPlane {FP_Top, FP_Bottom, FP_Left, FP_Right, FP_Near, FP_Far};
enum IntersectionType {IT_Inside, IT_Outside, IT_Intersect};


const char PlaneMasks[6] = { 1, 2, 4, 8, 16, 32 };

class Frustum
{
private:

	struct Plane
	{
		glm::vec4 plane;

		IntersectionType IntersectAABB(const AABoundingBox& box)
		{
			glm::vec3 h = box.getHalfDiagonal();
			glm::vec3 c = box.getCenter();

			float e = h.x * abs(plane.x) + h.y * abs(plane.y) + h.z * abs(plane.z);
			float s = c.x * plane.x + c.y * plane.y + c.z * plane.z + plane.w;

			if (s - e > 0) return IT_Outside;
			if (s + e < 0) return IT_Inside;
			return IT_Intersect;
		}

		IntersectionType IntersectAABB(const glm::vec3& c, const glm::vec3& h)
		{
			float e = h.x * abs(plane.x) + h.y * abs(plane.y) + h.z * abs(plane.z);
			float s = c.x * plane.x + c.y * plane.y + c.z * plane.z + plane.w;

			if (s - e > 0) return IT_Outside;
			if (s + e < 0) return IT_Inside;
			return IT_Intersect;
		}

		void Normalize()
		{
			float len = glm::sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
			plane /= len;
		}
	};

	Plane planes[6];

public:
	Frustum(glm::mat4 VP)
	{
		VP = glm::transpose(VP);
		planes[FP_Left  ].plane = -(VP[3] + VP[0]);
		planes[FP_Right ].plane = -(VP[3] - VP[0]);
		planes[FP_Bottom].plane = -(VP[3] + VP[1]);
		planes[FP_Top   ].plane = -(VP[3] - VP[1]);
		planes[FP_Near  ].plane = -(VP[3] + VP[2]);
		planes[FP_Far   ].plane = -(VP[3] - VP[2]);

		for (int n = 0; n < 6; n++)
		{
			planes[n].Normalize();
		}
	}					    

	inline const glm::vec4& getPlane(FrustumPlane plane)
	{
		return planes[plane].plane;
	}

	IntersectionType IntersectAABB(const AABoundingBox& box)
	{
		bool intersect = false;

		for (int k = 0; k < 6; k++)
		{
			IntersectionType it = planes[k].IntersectAABB(box);
			if (it == IT_Outside) return IT_Outside;
			else if (it == IT_Intersect) intersect = true;
		}

		if (intersect) return IT_Intersect;
		return IT_Inside;
	}

	IntersectionType IntersectAABB(const AABoundingBox& box, char& parentIn)
	{
		bool intersect = false;

		IntersectionType it;

		for (unsigned int k = 0; k < 6; ++k)
		{
			if ((1 << k) & parentIn)
			{
				continue;
			}

			it = planes[k].IntersectAABB(box);
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

			it = planes[k].IntersectAABB(c, h);
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