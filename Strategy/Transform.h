#pragma once

#include "ei\glm\glm.hpp"
#include "ei\glm\gtc\matrix_transform.hpp"
#include "ei\glm\gtx\quaternion.hpp"

class Transform
{
private:
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::quat Rotation;

	glm::mat4 Matrix;

	bool update;

	void makeMat();
	void makePRS();
public:
	Transform();
	Transform(glm::vec3& Pos, glm::quat& Rot, glm::vec3& Scl);

	inline const glm::vec3& getPos()
	{
		return Position;
	}

	inline const glm::quat& getRot()
	{
		return Rotation;
	}

	inline const glm::vec3& getScl()
	{
		return Scale;
	}

	inline void setPos(glm::vec3& Pos)
	{
		Position = Pos;
		update = true;
	}

	inline void setRot(glm::quat& Rot)
	{
		Rotation = Rot;
		update = true;
	}

	inline void setScl(glm::vec3& Scl)
	{
		Scale = Scl;
		update = true;
	}

	inline glm::mat4& getMat()
	{
		if(update)
			makeMat();
		return Matrix;
	}

	inline void setMat(glm::mat4& Mat)
	{
		Matrix = Mat;
		makePRS();
	}
};
