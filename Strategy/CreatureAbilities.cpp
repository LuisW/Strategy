#include "CreatureAbilities.h"
#include "AbilityEffects.h"

//AIAbilityState Tick(double deltaT){}
//void Init(){}
//AIArea getCastArea(){}

#pragma region BaseAbilities
class Ability_Move : public IAbility
{
private:

public:
	Ability_Move(EntityID _ent) : IAbility(_ent, 1.0f, 0, 0, FullAOE)
	{}

	void Init(const glm::vec3& castPos){}

	AIAbilityState Tick(double deltaT)
	{
		EntityID ent = getCaster();
		StatsComponent& stats = getStats(ent);
		Transform& trans = getTransform(ent);
		VelocityComponent& vel = getVelocity(ent);

		//Rotation
		bool done2 = false;
		trans.setRot(AISafeSlerpStep(trans.getRot(), getRotTarget(), stats.derived.turnspeed * deltaT, done2));

		//Movement
		float step = stats.derived.movementspeed;

		glm::vec3 front = getWorldSpaceFrontVec();
		glm::vec3 dir = getMoveTarget() - trans.getPos();
		dir.y = 0.0f;
		float dist = glm::length(dir);
		dir /= dist;

		if (glm::dot(front, dir) < 0.999f)
		{
			step *= 0.5f; //s = 0.5
		}

		vel.setVelocity(dir * step);

		step *= deltaT;

		bool done = false;
		if (step >= dist)
		{
			trans.setPos(getMoveTarget());
			done = true;
		}
		else
		{
			trans.setPos(trans.getPos() + dir * step);
			return AAS_Running;
		}

		if (done2 && done)
		{
			return AAS_Finished;
		}

		return AAS_Running;
	}
};

#pragma endregion

#pragma region CombatAbilities
class Ability_SlowingShot : public IAbility
{
private:
	glm::vec3 castPos;
	unsigned int cast;

public:
	Ability_SlowingShot(EntityID ent) : IAbility(ent, 50.0f, 0, 500)
	{
		setCastArea(AIArea(glm::dvec2(0.2, 50.0), glm::dvec2(-glm::radians(6.0), glm::radians(6.0)), glm::dvec2(M_PI * 0.25, M_PI* 0.75)));
	}

	void Init(const glm::vec3& _castPos)
	{
		castPos = _castPos;
	}

	AIAbilityState Tick(double deltaT)
	{
		Transform& trans = getTransform(getCaster());
		glm::vec3 dir = glm::normalize(castPos - trans.getPos());
		spawnBullet(trans.getPos(), dir * 50.0f, new SlowingShotProjectile(1000, 0.6f, getCaster()));
		return AAS_Finished;
	}
};

class Ability_Blink : public IAbility
{
private:

public:
	Ability_Blink(EntityID ent) : IAbility(ent, INFINITY, 500, 5000)
	{
		setCastArea(AIArea(glm::dvec2(3.0, 100.0), glm::dvec2(-M_PI*0.5, M_PI*0.5), glm::dvec2(0, M_PI)));
	}

	void Init(const glm::vec3& _castPos)
	{
	}

	AIAbilityState Tick(double deltaT)
	{
		EntityID ent = getCaster();
		Transform& trans = getTransform(ent);

		glm::vec3 target = getMoveTarget();
		target.y = trans.getPos().y;

		trans.setPos(target);
		trans.setRot(getRotTarget());

		return AAS_Finished;
	}
};

class Ability_ShockWave : public IAbility
{
private:

public:
	Ability_ShockWave(EntityID ent) : IAbility(ent, 30.0f, 000, 10000)
	{
		setCastArea(AIArea(glm::dvec2(3.0, 20.0), glm::dvec2(0.0, M_PI*2.0), glm::dvec2(M_PI * 0.5 - M_Err, M_PI * 0.5 + M_Err)));
	}

	void Init(const glm::vec3& _castPos)
	{
	}

	AIAbilityState Tick(double deltaT)
	{
		Transform& trans = getTransform(getCaster());
		spawnShockWave(trans.getPos(), 30.0f, new SlowingShotProjectile(4000, 0.3f, getCaster()));
		return AAS_Finished;
	}
};
#pragma endregion

CreatureAbilityManager::CreatureAbilityManager(GameData* data)
{
	IAbility::setGameData(data);

	AddBaseMoveAbility<Ability_Move>();
	AddAbility<Ability_Blink>(AF_Translation);
	AddAbility<Ability_SlowingShot>(AF_Damage | AF_Slow);
	AddAbility<Ability_ShockWave>(AF_Damage);
	//AddAbility<Ability_Charge>(AF_Translation | AF_Damage);
}
