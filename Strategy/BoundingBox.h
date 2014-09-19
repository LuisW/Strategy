#pragma once

#include "Ray.h"
#include "Transform.h"

#pragma warning(disable:4056)
#pragma warning(disable:4756)

enum IntersectionType { IT_Inside, IT_Outside, IT_Intersect };

class AABB
{
private:
	glm::vec3 min;
	glm::vec3 max;

public:
	AABB(const glm::vec3& _min, const glm::vec3& _max) : min(_min), max(_max)
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

class OBB
{
private:
	glm::vec4 u;
	glm::vec4 v;
	glm::vec4 w;
	glm::vec3 center;
	float radius;

public:
	OBB()
	{
	}

	OBB(const glm::vec4& _u, const glm::vec4& _v, const glm::vec4& _w, const glm::vec3& _center) : u(_u), v(_v), w(_w), center(_center)
		, radius(glm::sqrt(u.w * u.w + v.w * v.w + w.w * w.w))
	{
	}

	inline const glm::vec4& getU() const
	{
		return u;
	}

	inline const glm::vec4& getV() const
	{
		return v;
	}

	inline const glm::vec4& getW() const
	{
		return w;
	}

	inline const glm::vec3& getCenter() const
	{
		return center;
	}

	inline float getRadius() const
	{
		return radius;
	}

	inline void setU(const glm::vec4& _u) 
	{
		u = _u;
	}

	inline void setV(const glm::vec4& _v)
	{
		v = _v;
	}

	inline void setW(const glm::vec4& _w)
	{
		w = _w;
	}

	inline void setCenter(const glm::vec3& _center)
	{
		center = _center;
	}

	OBB transform(Transform& trans)
	{
		return OBB(glm::vec4(trans.getRot() * glm::vec3(u), u.w),
			       glm::vec4(trans.getRot() * glm::vec3(v), v.w), 
				   glm::vec4(trans.getRot() * glm::vec3(w), w.w),
				   center + trans.getPos());
	}

	IntersectionType IntersectLineSegment(const LineSegment& segment, float& sqrdist) const	//Returns "IT_Intersect" if the LineSegment intersects the OBB, "IT_Outside" otherwise
	{
		glm::mat3x4 axis = glm::mat3x4(getU(), getV(), getW());

		float tmin = -INFINITY;
		float tmax = INFINITY;

		Ray ray = Ray(segment.p2 - segment.p1, segment.p1);

		for (int n = 0; n < 3; n++)
		{
			float e = glm::dot(glm::vec3(axis[n]), ray.getA() - getCenter());
			float f = glm::dot(glm::vec3(axis[n]), ray.getDir());

			if (glm::abs(f) < 1e-06f)
			{
				if (e > axis[n].w || e < -axis[n].w)
					return IT_Outside;

				continue;
			}

			float t1 = (axis[n].w - e) / f;
			float t2 = (-axis[n].w - e) / f;

			if (t1 > t2)
			{
				float t3 = t1;
				t1 = t2;
				t2 = t3;
			}

			tmin = glm::max(tmin, t1);
			tmax = glm::min(tmax, t2);
		}

		if (tmin < tmax && tmin < 1.0f && tmax > 0.0f)
		{
			if (tmin <= 0.0f)
			{
				sqrdist = 0.0f;
			}
			else
			{
				sqrdist = tmin * tmin * glm::dot(ray.getDir(), ray.getDir());
			}

			return IT_Intersect;
		}

		return IT_Outside;
	}

	IntersectionType IntersectLineSegment(const LineSegment& segment) const	//Returns "IT_Intersect" if the LineSegment intersects the OBB, "IT_Outside" otherwise
	{
		glm::mat3x4 axis = glm::mat3x4(getU(), getV(), getW());

		float tmin = -INFINITY;
		float tmax = INFINITY;

		Ray ray = Ray(segment.p2 - segment.p1, segment.p1);

		for (int n = 0; n < 3; n++)
		{
			float e = glm::dot(glm::vec3(axis[n]), ray.getA() - getCenter());
			float f = glm::dot(glm::vec3(axis[n]), ray.getDir());

			if (glm::abs(f) < 1e-06f)
			{
				if (e > axis[n].w || e < -axis[n].w)
					return IT_Outside;

				continue;
			}

			float t1 = ( axis[n].w - e) / f;
			float t2 = (-axis[n].w - e) / f;

			if (t1 > t2)
			{
				float t3 = t1;
				t1 = t2;
				t2 = t3;
			}

			tmin = glm::max(tmin, t1);
			tmax = glm::min(tmax, t2);
		}

		if (tmin < tmax && tmin < 1.0f && tmax > 0.0f)
		{
			return IT_Intersect;
		}

		return IT_Outside;
	}

	IntersectionType IntersectRay(const Ray& ray, bool positiveOnly = false) const	//Returns "IT_Intersect" if the ray hits the OBB, "IT_Outside" otherwise
	{
		glm::mat3x4 axis = glm::mat3x4(getU(), getV(), getW());

		float tmin = -INFINITY;
		float tmax = INFINITY;

		for (int n = 0; n < 3; n++)
		{
			float e = glm::dot(glm::vec3(axis[n]), ray.getA() - getCenter());
			float f = glm::dot(glm::vec3(axis[n]), ray.getDir());

			if (glm::abs(f) < 1e-06f)
			{
				if (e > axis[n].w || e < -axis[n].w)
					return IT_Outside;

				continue;
			}

			float t1 = (axis[n].w - e) / f;
			float t2 = (-axis[n].w - e) / f;

			if (t1 > t2)
			{
				float t3 = t1;
				t1 = t2;
				t2 = t3;
			}

			if (t2 < 0.0f && positiveOnly)
			{
				return IT_Outside;
			}

			if (t1 < 0.0f && positiveOnly)
			{
				t1 = 0.0f;
			}

			tmin = glm::max(tmin, t1);
			tmax = glm::min(tmax, t2);
		}

		if (tmin < tmax)
		{
			return IT_Intersect;
		}

		return IT_Outside;
	}

	IntersectionType IntersectPoint(const glm::vec3& point) const	//Returns "IT_Inside" if the point is inside the OBB, "IT_Outside" otherwise
	{
		if (glm::abs(glm::dot(glm::vec3(getU()), point)) > getU().w)
		{
			return IT_Outside;
		}

		if (glm::abs(glm::dot(glm::vec3(getV()), point)) > getV().w)
		{
			return IT_Outside;
		}

		if (glm::abs(glm::dot(glm::vec3(getW()), point)) > getW().w)
		{
			return IT_Outside;
		}

		return IT_Inside;
	}

	IntersectionType IntersectOBB(const OBB& other) const	//Returns "IT_Intersect" if the OBBs intersect, "IT_Outside" otherwise
	{
		glm::mat3 A = glm::mat3(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z);
		glm::mat3 B = glm::mat3(glm::vec3(other.getU()), glm::vec3(other.getV()), glm::vec3(other.getW()));
		glm::mat3 C = A * B;

		glm::vec3 t = A * (other.getCenter() - getCenter());

		glm::vec3 radiiA = glm::vec3(u.w, v.w, w.w);
		glm::vec3 radiiB = glm::vec3(other.u.w, other.v.w, other.w.w);

		for (int n = 0; n < 3; n++)
		{
			if (glm::abs(t[n]) > radiiA[n] + other.u.w * glm::abs(C[n][0]) + other.v.w * glm::abs(C[n][1]) + other.w.w * glm::abs(C[n][2]))
			{
				return IT_Outside;
			}
		}

		for (int n = 0; n < 3; n++)
		{
			if (glm::abs(t.x * C[0][n] + t.y * C[1][n] + t.z * C[2][n]) > radiiB[n] + u.w * glm::abs(C[0][n]) + v.w * glm::abs(C[1][n]) + w.w * glm::abs(C[2][n]))
			{
				return IT_Outside;
			}
		}

		for (int a = 0; a < 3; a++)
		{
			for (int b = 0; b < 3; b++)
			{
				float ts = glm::abs(t[(a + 1) % 3] * C[(a + 2) % 3][b] - t[(a + 2) % 3] * C[(a + 1) % 3][b]);
				float da = radiiA[(a + 1) % 3] * glm::abs(C[(a + 2) % 3][b]) + radiiA[(a + 2) % 3] * glm::abs(C[(a + 1) % 3][b]);
				float db = radiiB[(b + 1) % 3] * glm::abs(C[a][(b + 2) % 3]) + radiiB[(b + 2) % 3] * glm::abs(C[a][(b + 1) % 3]);

				if (ts > da + db)
				{
					return IT_Outside;
				}
			}
		}

		return IT_Intersect;
	}
};