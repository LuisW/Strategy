#pragma once

#include "ei/glm/glm.hpp"
#include "BoundingBox.h"

enum FrustrumPlane {FP_Top, FP_Bottom, FP_Left, FP_Right, FP_Near, FP_Far};
enum IntersectionType {IT_Inside, IT_Outside, IT_Intersect};

class Frustrum
{
private:

	struct Plane
	{
		glm::vec4 plane;

		IntersectionType IntersectAABB(const AABoundingBox& box)
		{
			glm::vec3 h = box.getHalfDiagonal();

			float e = h.x * abs(plane.x) + h.y * abs(plane.y) + h.z * abs(plane.z);
			float s = glm::dot(box.getCenter(), glm::vec3(plane)) + plane.w;

			if (s - e > 0) return IT_Outside;
			if (s + e < 0) return IT_Inside;
			return IT_Intersect;
		}
	};

	const char PlaneMasks[6] = { 1, 2, 4, 8, 16, 32 };

	Plane planes[6];

public:
	Frustrum(glm::mat4 VP)
	{
		planes[FP_Left  ].plane = -(VP[3] + VP[0]);
		planes[FP_Right ].plane = -(VP[3] - VP[0]);
		planes[FP_Bottom].plane = -(VP[3] + VP[1]);
		planes[FP_Top   ].plane = -(VP[3] - VP[1]);
		planes[FP_Near  ].plane = -(VP[3] + VP[2]);
		planes[FP_Far   ].plane = -(VP[3] - VP[2]);
	}

	inline const glm::vec4& getPlane(FrustrumPlane plane)
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

	IntersectionType IntersectAABB(const AABoundingBox& box, char& parentIn, FrustrumPlane& failPlane)
	{
		bool intersect = false;

		IntersectionType it = planes[failPlane].IntersectAABB(box);
		if (it == IT_Outside) return IT_Outside;
		else if (it == IT_Intersect) intersect = true;

		for (int k = 0; k < 6; k++)
		{
			if (k == failPlane || PlaneMasks[k] & parentIn)
			{
				continue;
			}

			it = planes[k].IntersectAABB(box);
			if (it == IT_Outside)
			{
				failPlane = (FrustrumPlane)k;
				return IT_Outside; 
			}
			else if (it == IT_Intersect) intersect = true;
			else parentIn |= PlaneMasks[k];
		}

		if (intersect) return IT_Intersect;
		return IT_Inside;
	}
};