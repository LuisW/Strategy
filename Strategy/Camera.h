#pragma once

#include "ei\glm\glm.hpp"
#include "ei\glm\gtc\matrix_transform.hpp"
#include "ei\glm\gtx\quaternion.hpp"
#include "Frustrum.h"

class Camera
{
private:

	float fov;
	float aspect;
	float near;
	float far;

	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	mutable glm::mat4 view;
	mutable glm::mat4 projection;
	mutable glm::mat4 vp;
	mutable Frustrum viewFrustrum;

	glm::quat rotation;
	glm::vec3 position;

	mutable bool updateP;
	mutable bool updateV;
	mutable bool updateVP;

	void makeLocalSystem()
	{
		front = glm::vec3(0.0f, 0.0f, -1.0f) * rotation;
		up    = glm::vec3(0.0f, 1.0f,  0.0f) * rotation;
		right = glm::vec3(1.0f, 0.0f,  0.0f) * rotation;
	}

public:
	Camera(float _fov, float _aspect, float _near, float _far) : 
		fov(_fov), aspect(_aspect), near(_near), far(_far), projection(glm::perspective(fov, aspect, near, far)),
		updateP(false), updateV(true), updateVP(false), rotation(0.0f, 0.0f, 0.0f, 1.0f), position(0.0f), viewFrustrum(vp)
	{
		makeLocalSystem();
	}

	Camera(float _fov, float _aspect, float _near, float _far, glm::vec3 pos, glm::quat rot) :
		fov(_fov), aspect(_aspect), near(_near), far(_far), projection(glm::perspective(fov, aspect, near, far)), 
		updateP(false), updateV(true), updateVP(false), position(pos), rotation(rot), viewFrustrum(vp)
	{
		makeLocalSystem();
	}

	inline float getAspect() const
	{
		return aspect;
	}

	inline float getFoV() const
	{
		return fov;
	}

	inline float getNear() const
	{
		return near;
	}

	inline float getFar() const
	{
		return far;
	}

	inline const glm::mat4& getView() const
	{
		if (updateV)
		{
			view = glm::lookAt(position, position + front, up);
			updateV = 0;
			updateVP = true;
		}

		return view;
	}

	inline const glm::mat4& getProjection() const
	{
		if (updateP)
		{
			projection = glm::perspective(fov, aspect, near, far);
			updateP = false;
			updateVP = true;
		}

		return projection;
	}

	inline const glm::mat4& getViewProjection() const
	{
		if (updateVP || updateP || updateV)
		{
			vp = getProjection() * getView();
			viewFrustrum = Frustrum(vp);
			updateVP = false;
		}

		return vp;
	}

	inline void setAspect(float _aspect)
	{
		aspect = _aspect;
		updateP = true;
	}

	inline void setFov(float _fov)
	{
		fov = _fov;
		updateP = true;
	}

	inline void setNear(float _near)
	{
		near = _near;
		updateP = true;
	}

	inline void setFar(float _far)
	{
		far = _far;
		updateP = true;
	}

	inline const glm::vec3& getPos() const
	{
		return position;
	}

	inline void setPos(const glm::vec3& pos)
	{
		position = pos;
		updateV = true;
	}

	inline const glm::quat& getRot() const
	{
		return rotation;
	}

	inline void setRot(const glm::quat& rot)
	{
		rotation = rot;
		makeLocalSystem();
		updateV = true;
	}

	inline void Rotate(const glm::quat& rot)
	{
		rotation = rotation * rot;
		makeLocalSystem();
		updateV = true;
	}

	inline void Move(float dist)
	{
		position += dist * front;
		updateV = true;
	}

	inline void Strafe(float dist)
	{
		position += dist * right;
		updateV = true;
	}

	inline void Translate(const glm::vec3& trans)
	{
		position += trans;
		updateV = true;
	}

	inline const glm::vec3& getRight()
	{
		return right;
	}

	inline const glm::vec3& getUp()
	{
		return up;
	}

	inline const glm::vec3& getFront()
	{  
		return front;
	}

	inline Frustrum& getFrustrum()
	{
		return viewFrustrum;
	}
};