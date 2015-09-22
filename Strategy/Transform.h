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
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;

	mutable glm::mat4	m_matrix;
	mutable bool		m_update;

	void makeMat() const;
	void makePRS();
public:
	Transform();
	Transform(const glm::vec3& Pos, const glm::quat& Rot, const glm::vec3& Scl);

	inline const glm::vec3& getPos() const
	{
		return m_position;
	}

	inline const glm::quat& getRot() const
	{
		return m_rotation;
	}

	inline const glm::vec3& getScl() const
	{
		return m_scale;
	}

	inline void setPos(const glm::vec3& Pos)
	{
		m_position = Pos;
		m_update = true;
	}

	inline void setRot(const glm::quat& Rot)
	{
		m_rotation = Rot;
		m_update = true;
	}

	inline void setScl(const glm::vec3& Scl)
	{
		m_scale = Scl;
		m_update = true;
	}

	inline const glm::mat4& getMat() const
	{
		if (m_update)
		{
			makeMat();
			m_update = false;
		}

		return m_matrix;
	}

	inline void setMat(const glm::mat4& Mat)
	{
		m_matrix = Mat;
		makePRS();
	}
};
