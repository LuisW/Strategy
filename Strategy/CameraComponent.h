#pragma once

#include "Component.h"
#include "Camera.h"

class CameraComponent : public Component, public Camera
{
private:
public:
	CameraComponent(EntityID owner, const std::string& name, float _fov, float _aspect, float _near, float _far) 
		: Component(CT_Camera, owner, name), Camera(_fov, _aspect, _near, _far)
	{}

	CameraComponent(EntityID owner, const std::string& name, float _fov, float _aspect, float _near, float _far, const Transform& _view)
		: Component(CT_Camera, owner, name), Camera(_fov, _aspect, _near, _far, _view)
	{}

	static ComponentType getComponentType()
	{
		return CT_Camera;
	}
};