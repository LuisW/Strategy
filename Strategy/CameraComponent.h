#pragma once

#include "Component.h"
#include "Camera.h"

class CameraComponent : public Component, public Camera
{
private:
public:
	CameraComponent(EntityID owner, const std::string& name, float fov, float aspect, float near, float far) 
		: Component(CT_CAMERA, owner, name), Camera(fov, aspect, near, far)
	{}

	CameraComponent(EntityID owner, const std::string& name, float fov, float aspect, float near, float far, glm::vec3 pos, glm::quat rot)
		: Component(CT_CAMERA, owner, name), Camera(fov, aspect, near, far, pos, rot)
	{}

	static ComponentType getComponentType()
	{
		return CT_CAMERA;
	}
};