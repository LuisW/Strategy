#pragma once

#include "ProjectileComponent.h"

class GroundWaveComponent : public ProjectileComponent
{
private:
	float radius;
	float maxRadius;
	float expVel;

public:
	GroundWaveComponent(EntityID owner, EntityID _source, float _expVel, float _radius, float _maxRadius, IProjectileEffect* _effect)
		:  radius(_radius), maxRadius(_maxRadius), expVel(_expVel), ProjectileComponent(owner, _source, _effect, PT_GroundWave)
	{
	}

	float getMaxRadius() const
	{
		return maxRadius;
	}

	float getRadius() const
	{
		return radius;
	}

	void setRadius(float _radius)
	{
		radius = _radius;
	}

	float getExpansionVelocity() const
	{
		return expVel;
	}

	void setExpansionVelocity(float _expVel)
	{
		expVel = _expVel;
	}

	~GroundWaveComponent()
	{
	}
};