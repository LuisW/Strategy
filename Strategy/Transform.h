#pragma once

#include "ei\glm\glm.hpp"
#include "ei\glm\gtc\matrix_transform.hpp"
#include "ei\glm\gtx\quaternion.hpp"

#ifndef M_PI
	#define M_PI 3.14159265358979323846
	#define M_PI_f 3.14159265358979323846f
#endif

#define M_Err 1e-06f

class Transform
{
private:
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::quat Rotation;

	mutable glm::mat4 Matrix;

	mutable bool update;

	void makeMat() const;
	void makePRS();
public:
	Transform();
	Transform(const glm::vec3& Pos, const glm::quat& Rot, const glm::vec3& Scl);

	inline const glm::vec3& getPos() const
	{
		return Position;
	}

	inline const glm::quat& getRot() const
	{
		return Rotation;
	}

	inline const glm::vec3& getScl() const
	{
		return Scale;
	}

	inline void setPos(const glm::vec3& Pos)
	{
		Position = Pos;
		update = true;
	}

	inline void setRot(const glm::quat& Rot)
	{
		Rotation = Rot;
		update = true;
	}

	inline void setScl(const glm::vec3& Scl)
	{
		Scale = Scl;
		update = true;
	}

	inline const glm::mat4& getMat() const
	{
		if (update)
		{
			makeMat();
			update = false;
		}

		return Matrix;
	}

	inline void setMat(const glm::mat4& Mat)
	{
		Matrix = Mat;
		makePRS();
	}
};
