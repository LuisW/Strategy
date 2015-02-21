#pragma once

#include "IStatEffect.h"
#include "ProjectileComponent.h"
#include "GroundWaveComponent.h"

class SlowEffect : public IStatEffect
{
private:
	float amount;

public:
	SlowEffect(unsigned int duration, float _amount) : IStatEffect(duration + SDL_GetTicks(), false), amount(_amount)
	{
	}

	bool derivedEffect(StatsStruct& stats, EntityID ent, double deltaT)
	{
		return false;
	}
	
	bool statEffect(StatsStruct& stats, EntityID ent, double deltaT)
	{
		stats.agility.current -= stats.agility.current * amount;
		return false;
	}
};

class ChargeEffect : public IStatEffect
{
private:
	float speed;

public:
	ChargeEffect(float _speed, unsigned int duration) : IStatEffect(duration + SDL_GetTicks(), false), speed(_speed)
	{
	}

	bool derivedEffect(StatsStruct& stats, EntityID ent, double deltaT)
	{
		stats.derived.movementspeed = speed;
		return false;
	}

	bool statEffect(StatsStruct& stats, EntityID ent, double deltaT)
	{
		return false;
	}
};

class SlowingShotProjectile : public IProjectileEffect
{
private:
	unsigned int duration;
	float amount;

public:
	SlowingShotProjectile(unsigned int _duration, float _amount, EntityID _owner) : amount(_amount), duration(_duration)
	{}

	bool Filter(EntityID ent, EntityManager& entityManager)
	{
		return entityManager.entityHasComponent(ent, CT_Stats) && 
				!entityManager.entityHasComponent(ent, CT_CreatureAI);
	}

	bool Effect(EntityID ent, EntityManager& entityManager, double deltaT)
	{
		entityManager.entityGetComponent<StatsComponent>(ent).AddEffect(new SlowEffect(duration, amount));
		return true;
	}
};

class ShockWaveProjectile : public IProjectileEffect
{
private:
	EntityID owner;

public:
	ShockWaveProjectile() : owner(0)
	{}

	void setOwner(EntityID _owner)
	{
		owner = _owner;
	}

	bool Filter(EntityID ent, EntityManager& entityManager)
	{
		return entityManager.entityHasComponent(ent, CT_Transform) &&
			!entityManager.entityHasComponent(ent, CT_CreatureAI);
	}

	bool Effect(EntityID ent, EntityManager& entityManager, double deltaT)
	{
		if (owner == 0 || !entityManager.entityHasComponent(owner, CT_Projectile) || entityManager.entityGetComponent<ProjectileComponent>(owner).getProjectileType() != PT_GroundWave)
			return true;

		Transform& target = entityManager.entityGetComponent<TransformComponent>(ent);
		Transform& trans = entityManager.entityGetComponent<TransformComponent>(owner);
		GroundWaveComponent& wave = entityManager.entityGetComponent<GroundWaveComponent>(owner);

		glm::vec3 dv = target.getPos() - trans.getPos();
		dv.y = 0.0f;
		dv *= (float) deltaT * wave.getExpansionVelocity() / glm::length(dv);

		target.setPos(target.getPos() + dv);

		if (entityManager.entityHasComponent(ent, CT_Stats))
			entityManager.entityGetComponent<StatsComponent>(ent).AddEffect(new SlowEffect(100, 1.0f));

		return false;
	}
};