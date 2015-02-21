#pragma once

#include "AIHelp.h"
#include "StatsComponent.h"
#include "VelocityComponent.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

enum AbilityFunction { AF_Damage = 1, AF_Stun = 2, AF_Slow = 4, AF_Heal = 8, AF_Translation = 16, AF_PushEnemy = 32, AF_PullEnemy = 64 };

class CreatureAbilityManager;
class GameData;
class IProjectileEffect;
typedef unsigned int EntityID;

struct AbilityInfo
{
	unsigned int functions;
	AbilityInfo() : functions(0)
	{}
};

class IAbility
{
private:
	AbilityInfo* info;
	static GameData* data;

	friend CreatureAbilityManager;
	static void setGameData(GameData* _data);

	unsigned int castPoint;
	unsigned int readyPoint;

	bool cdset;

	EntityID ent;
	float skillVel;
	unsigned int castTime;
	unsigned int cooldown;
	AIArea castArea;

protected:
	EntityID getCaster();
	StatsComponent& getStats(EntityID ent);
	TransformComponent& getTransform(EntityID ent);
	VelocityComponent& getVelocity(EntityID ent);
	CollisionComponent& getCollision(EntityID ent);
	EntityID getPlayer();
	float getDistToPlayer();
	const glm::vec3& getDirectionToPlayer();
	const glm::vec3& getWorldSpaceFrontVec();
	const glm::vec3& getMoveTarget();
	const glm::quat& getRotTarget();
	void setCastArea(const AIArea& area);
	void spawnBullet(const glm::vec3& pos, const glm::vec3& vel, IProjectileEffect* effect);
	void spawnShockWave(const glm::vec3& pos, float vel, IProjectileEffect* effect);

public:
	IAbility(EntityID _ent, float _skillVel, unsigned int _castTime, unsigned int _cooldown, AIArea _castArea)
		: ent(_ent), skillVel(_skillVel), castTime(_castTime), cooldown(_cooldown), castArea(_castArea)
	{}
	
	IAbility(EntityID _ent, float _skillVel, unsigned int _castTime, unsigned int _cooldown)
		: ent(_ent), skillVel(_skillVel), castTime(_castTime), cooldown(_cooldown), castArea(FullAOE)
	{}

	const AbilityInfo& getInfo() { return *info; }
	void setInfo(AbilityInfo& _info) { info = &_info; }

	virtual AIAbilityState Tick(double deltaT) = 0;
	virtual void Init(const glm::vec3& castPos) = 0;
	
	void Cast(const glm::vec3& castPos)
	{
		castPoint = SDL_GetTicks() + castTime;
		cdset = false;
		Init(castPos);
	}

	void Cast()
	{
		castPoint = SDL_GetTicks() + castTime;
		cdset = false;
		Init(getTransform(ent).getPos());
	}

	AIAbilityState _Tick(double deltaT)
	{
		if (SDL_GetTicks() >= castPoint)
		{
			if (!cdset)
			{
				readyPoint = cooldown + SDL_GetTicks();
				cdset = true;
			}

			return Tick(deltaT);
		}

		return AAS_Precast;
	}

	float getSkillVelocity() const
	{
		return skillVel;
	}

	unsigned int getCastTime() const
	{
		return castTime;
	}

	unsigned int getCooldown() const
	{
		return cooldown;
	}

	unsigned int getRemainCooldown() const
	{
		return glm::max((int)readyPoint - (int)SDL_GetTicks(), 0);
	}

	bool isReady() const
	{
		unsigned int t = SDL_GetTicks();
		return t >= readyPoint;
	}

	const AIArea& getCastArea() const
	{
		return castArea;
	}
}; 