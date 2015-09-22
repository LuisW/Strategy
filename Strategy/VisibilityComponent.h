#pragma once

#include "Component.h"
#include "BoundingBox.h"

class VisibilityComponent : public Component
{
private:
	bool visible;
	AABoundingBox boundingBox;

public:
	VisibilityComponent(EntityID owner, const glm::vec3& min, const glm::vec3& max) : Component(CT_VISIBILITY, owner, ""), boundingBox(min, max), visible(true)
	{
	}

	inline bool getVisibility()
	{
		return visible;
	}

	inline const AABoundingBox& getBoundingBox() const
	{
		return boundingBox;
	}

	void setBoundingBox(const AABoundingBox& AABB)
	{
		boundingBox = AABB;
	}

	void setVisibility(bool _visible)
	{
		visible = _visible;
	}

	static ComponentType getComponentType()
	{
		return CT_VISIBILITY;
	}

};