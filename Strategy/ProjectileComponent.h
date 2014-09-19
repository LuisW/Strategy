#pragma once

#include "Component.h"
#include "CollisionSystem.h"

class IProjectileEffect : public ICollisionFilter
{
private:
public:
	virtual bool Filter(EntityID ent){ return true; }
	virtual bool Effect(EntityID ent) = 0;
	virtual ~IProjectileEffect(){};
};

class ProjectileComponent : public Component
{
private:
	IProjectileEffect* effect;
	glm::vec3 pos;
	glm::vec3 prevPos;

public:
	ProjectileComponent(EntityID owner, IProjectileEffect* _effect) : effect(_effect), Component(CT_Projectile, owner, "")
	{
	}

	const glm::vec3& getPos() const
	{
		return pos;
	}

	const glm::vec3& getPrevPos() const
	{
		return prevPos;
	}

	void setPos(const glm::vec3& _pos)
	{
		pos = _pos;
	}

	void setPrevPos(const glm::vec3& _prevPos)
	{
		prevPos = _prevPos;
	}

	IProjectileEffect& getEffect()
	{
		return *effect;
	}

	static ComponentType getComponentType()
	{
		return CT_Projectile;
	}

	~ProjectileComponent()
	{
		delete effect;
	}
};