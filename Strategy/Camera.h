#pragma once

#include "Transform.h"

class Camera
{
private:

	float fov;
	float aspect;
	float near;
	float far;
	Transform view;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	mutable glm::mat4 projection;
	mutable glm::mat4 vp;
	mutable bool update;
	mutable bool updatevp;

	void makeLocalSystem()
	{
		front = glm::vec3(0.0f, 0.0f, -1.0f) * view.getRot();
		up	  = glm::vec3(0.0f, 1.0f,  0.0f) * view.getRot();
		right = glm::vec3(1.0f, 0.0f,  0.0f) * view.getRot();
	}

public:
	Camera(float _fov, float _aspect, float _near, float _far) : 
		fov(_fov), aspect(_aspect), near(_near), far(_far), projection(glm::perspective(fov, aspect, near, far)), update(false)
	{
		vp = projection * view.getMat();
		makeLocalSystem();
	}

	Camera(float _fov, float _aspect, float _near, float _far, const Transform& _view) :
		fov(_fov), aspect(_aspect), near(_near), far(_far), projection(glm::perspective(fov, aspect, near, far)), view(_view), update(false)
	{
		vp = projection * view.getMat();
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
		return view.getMat();
	}

	inline const glm::mat4& getProjection() const
	{
		if (update)
		{
			projection = glm::perspective(fov, aspect, near, far);
			update = false;
			updatevp = true;
		}

		return projection;
	}

	inline const glm::mat4& getViewProjection() const
	{
		if (update || updatevp)
		{
			vp = getProjection() * view.getMat();
			updatevp = false;
		}

		return vp;
	}

	inline void setAspect(float _aspect)
	{
		aspect = _aspect;
		update = true;
	}

	inline void setFov(float _fov)
	{
		fov = _fov;
		update = true;
	}

	inline void setNear(float _near)
	{
		near = _near;
		update = true;
	}

	inline void setFar(float _far)
	{
		far = _far;
		update = true;
	}

	inline const glm::vec3& getPos() const
	{
		return view.getPos();
	}

	inline void setPos(const glm::vec3& pos)
	{
		view.setPos(pos);
	}

	inline const glm::quat& getRot() const
	{
		return view.getRot();
	}

	inline void setRot(const glm::quat& rot)
	{
		view.setRot(rot);
		makeLocalSystem();
	}

	inline void Rotate(const glm::quat& rot)
	{
		view.setRot(view.getRot() * rot);
		makeLocalSystem();
	}

	inline void Move(float dist)
	{
		view.setPos(view.getPos() + dist * front);
	}

	inline void Strafe(float dist)
	{
		view.setPos(view.getPos() + dist * right);
	}

	inline void Translate(const glm::vec3& trans)
	{
		view.setPos(view.getPos() + trans);
	}
};