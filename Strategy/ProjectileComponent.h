#pragma once

#include "Component.h"
#include "CollisionSystem.h"

class IProjectileEffect : public ICollisionFilter
{
private:
public:
	virtual bool Filter(EntityID ent, EntityManager& entityManager){ return true; }
	virtual bool Effect(EntityID ent, EntityManager& entityManager, double deltaT) = 0;
	virtual ~IProjectileEffect(){};
};

enum ProjectileType {PT_Bullet, PT_GroundWave, PT_Unused}; //PT_Unused must be last

class ProjectileComponent : public Component
{
private:
	IProjectileEffect* effect;
	EntityID source;
	ProjectileType type;

protected:
	ProjectileComponent(EntityID owner, EntityID _source, IProjectileEffect* _effect, ProjectileType _type) 
		: source(_source), effect(_effect), type(_type), Component(CT_Projectile, owner, "")
	{
	}

public:
	ProjectileType getProjectileType()
	{
		return type;
	}

	EntityID getSource() const
	{
		return source;
	}

	void setSource(EntityID _source)
	{
		source = _source;
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