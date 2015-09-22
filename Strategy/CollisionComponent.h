#pragma once

#include "Component.h"
#include "BoundingBox.h"

class CollisionComponent : public Component
{
private:
	OBB m_collisionBox;

public:

	CollisionComponent(EntityID owner, const OBB& collisionBox) : m_collisionBox(collisionBox), Component(CT_COLLISION, owner, "")
	{
	}

	OBB& getCollisionVolume()
	{
		return m_collisionBox;
	}

	static ComponentType getComponentType()
	{
		return CT_COLLISION;
	}

	~CollisionComponent()
	{
	}
};