#pragma once

#include "ei\glm\glm.hpp"
#include "ei\glm\gtc\matrix_transform.hpp"
#include "ei\glm\gtx\quaternion.hpp"
#include "Frustum.h"

class Camera
{
private:

	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;

	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;

	mutable glm::mat4 m_view;
	mutable glm::mat4 m_projection;
	mutable glm::mat4 m_vp;
	
	mutable Frustum	m_viewFrustum;

	glm::quat m_rotation;
	glm::vec3 m_position;

	float m_yaw;
	float m_pitch;
	float m_roll;

	mutable bool m_updateP;
	mutable bool m_updateV;
	mutable bool m_updateVP;

	bool m_FlyCam;

	void makeLocalSystem()
	{
		m_front = glm::vec3(0.0f, 0.0f, -1.0f) * m_rotation;
		m_up    = glm::vec3(0.0f, 1.0f,  0.0f) * m_rotation;
		m_right = glm::vec3(1.0f, 0.0f,  0.0f) * m_rotation;
	}

public:
	Camera(float fov, float aspect, float near, float far) : 
		m_fov(fov), m_aspect(aspect), m_near(near), m_far(far), m_projection(glm::perspective(m_fov, m_aspect, m_near, m_far)),
		m_updateP(false), m_updateV(true), m_updateVP(false), m_rotation(0.0f, 0.0f, 0.0f, 1.0f), m_position(0.0f), m_viewFrustum(m_vp),
		m_yaw(0.0f), m_pitch(0.0f), m_roll(0.0f), m_FlyCam(false)
	{
		makeLocalSystem();
	}

	Camera(float fov, float aspect, float near, float far, glm::vec3 pos, glm::quat rot) :
		m_fov(fov), m_aspect(aspect), m_near(near), m_far(far), m_projection(glm::perspective(m_fov, m_aspect, m_near, m_far)), 
		m_updateP(false), m_updateV(true), m_updateVP(false), m_position(pos), m_rotation(rot), m_viewFrustum(m_vp),
		m_yaw(0.0f), m_pitch(0.0f), m_roll(0.0f), m_FlyCam(false)
	{
		makeLocalSystem();
	}

	inline float getAspect() const
	{
		return m_aspect;
	}

	inline float getFoV() const
	{
		return m_fov;
	}

	inline float getNear() const
	{
		return m_near;
	}

	inline float getFar() const
	{
		return m_far;
	}

	inline const glm::mat4& getView() const
	{
		if (m_updateV)
		{
			m_view = glm::lookAt(m_position, m_position + m_front, m_up);
			m_updateV = 0;
			m_updateVP = true;
		}

		return m_view;
	}

	inline const glm::mat4& getProjection() const
	{
		if (m_updateP)
		{
			m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
			m_updateP = false;
			m_updateVP = true;
		}

		return m_projection;
	}

	inline const glm::mat4& getViewProjection() const
	{
		if (m_updateVP || m_updateP || m_updateV)
		{
			m_vp = getProjection() * getView();
			m_viewFrustum = Frustum(m_vp);
			m_updateVP = false;
		}

		return m_vp;
	}

	inline void setAspect(float _aspect)
	{
		m_aspect = _aspect;
		m_updateP = true;
	}

	inline void setFov(float _fov)
	{
		m_fov = _fov;
		m_updateP = true;
	}

	inline void setNear(float _near)
	{
		m_near = _near;
		m_updateP = true;
	}

	inline void setFar(float _far)
	{
		m_far = _far;
		m_updateP = true;
	}

	inline const glm::vec3& getPos() const
	{
		return m_position;
	}

	inline void setPos(const glm::vec3& pos)
	{
		m_position = pos;
		m_updateV = true;
	}

	inline void setYPos(float height)
	{
		m_position.y = height;
		m_updateV = true;
	}

	inline const glm::quat& getRot() const
	{
		return m_rotation;
	}

	inline void AddYawPitch(float deltaYaw, float deltaPitch)
	{
		m_yaw += deltaYaw;
		m_pitch += deltaPitch;

		if (m_pitch > 60.0f)
			m_pitch = 60.0f;

		if (m_pitch < -60.0f)
			m_pitch = -60.0f;

		m_rotation = glm::angleAxis(m_yaw, glm::vec3(0.0f, -1.0f, 0.0f));
		
		m_right = m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
		
		m_rotation = glm::angleAxis(-m_pitch, m_right) * m_rotation;

		m_front = m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
		m_up = m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);

		m_updateV = true;
	}

	inline void Move(float dist)
	{
		if (m_FlyCam)
		{
			m_position += dist * m_front;
		}
		else
		{
			glm::vec3 mov = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
			m_position += dist * mov;
		}

		m_updateV = true;
	}

	inline void Strafe(float dist)
	{
		m_position += dist * m_right;
		m_updateV = true;
	}

	inline void Translate(const glm::vec3& trans)
	{
		m_position += trans;
		m_updateV = true;
	}

	inline const glm::vec3& getRight()
	{
		return m_right;
	}

	inline const glm::vec3& getUp()
	{
		return m_up;
	}

	inline const glm::vec3& getFront()
	{  
		return m_front;
	}

	inline Frustum& getFrustum() const
	{
		getViewProjection();
		return m_viewFrustum;
	}

	inline void setFlying(bool flying)
	{
		m_FlyCam = flying;
	}
};