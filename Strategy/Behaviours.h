#pragma once

#include "BehaviourManager.h"

#include "CameraComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "CreatureAIComponent.h"
#include "CreatureManager.h"
#include "ProjectileComponent.h"
#include "VitalsComponent.h"

class BehaviourPassStruct
{
private:
	EntityID ent;
	CreatureAIComponent* AIcomp;
	TransformComponent* trans;
	RenderComponent* rend;
	StatsComponent* stats;
	VitalsComponent* vitals;
	Camera* player;
	double deltaT;

public:

	EntityID getEntityID()
	{
		return ent;
	}

	CreatureAIComponent& getAIComp()
	{
		return *AIcomp;
	}

	TransformComponent& getTrans()
	{
		return *trans;
	}

	RenderComponent& getRender()
	{
		return *rend;
	}

	StatsComponent& getStats()
	{
		return *stats;
	}

	VitalsComponent& getVitals()
	{
		return *vitals;
	}

	Camera& getPlayer()
	{
		return *player;
	}

	double getDeltaT()
	{
		return deltaT;
	}

	void setEntityID(EntityID _ent)
	{
		ent = _ent;
	}

	void setAIComp(CreatureAIComponent& _AIcomp)
	{
		AIcomp = &_AIcomp;
	}

	void setTrans(TransformComponent& _trans)
	{
		trans = &_trans;
	}

	void setRender(RenderComponent& _rend)
	{
		rend = &_rend;
	}

	void setStats(StatsComponent& _stats)
	{
		stats = &_stats;
	}

	void setVitals(VitalsComponent& _vitals)
	{
		vitals = &_vitals;
	}

	void setPlayer(Camera& _player)
	{
		player = &_player;
	}

	void setDeltaT(double _deltaT)
	{
		deltaT = _deltaT;
	}
};

class IBehaviour
{
private:
public:
	virtual void Run(BehaviourPassStruct& data) = 0;
	virtual ~IBehaviour(){};
};

class IAbility : public IBehaviour
{
private:
public:
	virtual void Run(BehaviourPassStruct& data) = 0;
	virtual float getRange() = 0;
	virtual bool isReady() = 0;
	virtual ~IAbility(){};
};

const glm::vec3 CreatureFront(-1.0f, 0.0f, 0.0f);

glm::quat AISafeSlerpStep(const glm::quat& x, const glm::quat& y, float maxStep, bool& finished);
glm::quat AIFindQuatToTarget(const glm::vec3& target, Transform& trans);

float AIStepTowardsTarget(const glm::vec3& target, float dist, Transform& trans);
void AIStepInDirection(const glm::vec3& dir, float dist, Transform& trans);

class AIRotator
{
private:
	glm::quat start;
	glm::quat currRot;
	float progress;
	bool done;

public:
	AIRotator();
	void setTarget(glm::vec3& target, Transform& trans);
	bool isDone();
	void step(float angle, Transform& trans);
};

class Idle_Scan : IBehaviour
{
private:
	glm::vec3 target;
	unsigned int lookTimer;

	float cphi;
	unsigned int steps;

	AIRotator rotator;

public:
	Idle_Scan(glm::vec3& location)
	{
		lookTimer = 0;

		cphi = (float)randuint(SDL_GetTicks() % 10000, 0, 10000);
		cphi *= 0.00031416f * 2.0f;
		steps = 0;
	}

	void Run(BehaviourPassStruct& data)
	{
		if (SDL_GetTicks() < lookTimer + 1000)
			return;

		rotator.step((float)data.getDeltaT() * data.getStats().derived.turnspeed, data.getTrans());

		if (rotator.isDone())
		{
			if (steps >= 10)
			{
				cphi = (float)randuint(SDL_GetTicks() % 10000, 0, 10000);
				cphi *= 0.00031416f * 2.0f;
				steps = 0;
			}

			float tphi = (float)randuint(SDL_GetTicks() % 10000, 0, 1000);
			tphi -= 500.0f;
			tphi *= 0.0031416f * 0.25f;

			tphi += cphi;

			target = glm::vec3(-glm::cos(tphi), 0.0f, glm::sin(tphi));

			lookTimer = SDL_GetTicks();

			rotator.setTarget(target, data.getTrans());

			steps++;

			return;
		}
	}

	~Idle_Scan()
	{}
};

class Idle_Roam : IBehaviour
{
private:
	glm::vec3 target;
	glm::vec3 center;
	
	AIRotator rotator;

public:
	Idle_Roam(glm::vec3& location) : target(location), center(location)
	{
	}

	void Run(BehaviourPassStruct& data)
	{
		if (rotator.isDone())
		{
			float dist = AIStepTowardsTarget(target, data.getStats().derived.movementspeed * (float)data.getDeltaT(), data.getTrans());

			while (dist < 1e-06)
			{
				float radius = (float)randuint(SDL_GetTicks(), 0, 20);
				float phi = (float)randuint(SDL_GetTicks(), 0, 1000);
				phi *= 0.0031416f * 2.0f;

				target = center + glm::vec3(glm::cos(phi) * radius, 0.0f, glm::sin(phi) * radius);

				glm::vec3 distv = target - data.getTrans().getPos();

				dist = glm::length(distv);

				if (dist < 1e-06)
				{
					continue;
				}

				rotator.setTarget(distv / dist, data.getTrans());
			}
		}
		else
		{
			rotator.step((float)data.getDeltaT() * data.getStats().derived.turnspeed, data.getTrans());
		}
	}

	~Idle_Roam()
	{}
};

class RangeSetup_CirclePlayer : IBehaviour
{
private:
	AIRotator rotator;

public:
	RangeSetup_CirclePlayer(glm::vec3& location)
	{
	}

	void Run(BehaviourPassStruct& data)
	{
		glm::vec3 distv = data.getPlayer().getPos() - data.getTrans().getPos();
		distv.y = 0.0f;
		float dist = glm::length(distv);
		distv /= dist;
		dist -= 10.0f;

		if (dist != 0.0f)
		{
			if (glm::abs(dist) > data.getStats().derived.movementspeed * (float)data.getDeltaT())
			{
				dist = data.getStats().derived.movementspeed * (float)data.getDeltaT() * glm::sign(dist);
			}

			AIStepInDirection(distv, dist, data.getTrans());
		}

		rotator.setTarget(distv, data.getTrans());
		rotator.step((float)data.getDeltaT() * data.getStats().derived.turnspeed, data.getTrans());
	}

	~RangeSetup_CirclePlayer()
	{}
};

class SlowingShotProjectile : IProjectileEffect
{
private:
	unsigned int duration;
	float amount;

public:
	SlowingShotProjectile(unsigned int _duration, float _amount) : amount(_amount), duration(_duration)
	{}

	bool Filter(EntityID ent)
	{
		return true;
	}

	bool Effect(EntityID ent)
	{
		return true;
	}
};

class Attack_SlowingShot : IAbility
{
private:
	unsigned int cooldown;

public:
	Attack_SlowingShot(glm::vec3& location)
	{
	}

	bool isReady()
	{
		return SDL_GetTicks() >= cooldown;
	}

	void Run(BehaviourPassStruct& data)
	{
		if (!isReady())
			return;

		cooldown = SDL_GetTicks() + 1500;
	}

	~Attack_SlowingShot()
	{}
};