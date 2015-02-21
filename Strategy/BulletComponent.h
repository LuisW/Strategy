#pragma once

#include "ProjectileComponent.h"

class BulletComponent : public ProjectileComponent
{
private:
	float travelDist;
	float maxTravelDist;

public:
	BulletComponent(EntityID owner, EntityID _source, const glm::vec3 _pos, IProjectileEffect* _effect)
		: travelDist(0.0f), ProjectileComponent(owner, _source, _effect, PT_Bullet)
	{
	}

	void addTravelDist(float delta)
	{
		travelDist += delta;
	}

	float getTravelDist()
	{
		return travelDist;
	}

	~BulletComponent()
	{
	}
};