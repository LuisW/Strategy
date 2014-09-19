#pragma once

#include "Component.h"
#include "BoundingBox.h"

class CollisionComponent : public Component
{
private:
	OBB collisionBox;

public:

	CollisionComponent(EntityID owner, const OBB& _collisionBox) : collisionBox(_collisionBox), Component(CT_Collision, owner, "")
	{
	}

	OBB& getCollisionVolume()
	{
		return collisionBox;
	}

	static ComponentType getComponentType()
	{
		return CT_Collision;
	}

	~CollisionComponent()
	{
	}
};