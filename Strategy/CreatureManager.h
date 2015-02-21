#pragma once

#include <functional>
#include <vector>
#include "AssetManager.h"
#include "IStatEffect.h"
#include "CreatureAbilities.h"
#include "ProcGenHelp.h"

typedef unsigned int CreatureID;

struct CreatureStat
{
	float baseValue;
	float scale;
};

struct Creature
{
	unsigned int seed;

	float perception;
	CreatureStat agility;
	CreatureStat intelligence;
	CreatureStat stamina;
	CreatureStat vitality;
	CreatureStat strength;
	CreatureStat power;

	std::vector<AbilityID> abilities;
	AbilityID defaultMove;

	Creature(unsigned int _seed, CreatureAbilityManager& abilityManager) : seed(_seed)
	{
		int r = randuint(seed, 0, 1000);
		perception = glm::pow((float)r / 500.0f - 1.0f, 3.0f);

		agility.baseValue = randStatBase(_seed * 2);
		intelligence.baseValue = randStatBase(_seed * 3);
		stamina.baseValue = randStatBase(_seed * 4);
		vitality.baseValue = randStatBase(_seed * 5);
		strength.baseValue = randStatBase(_seed * 6);
		power.baseValue = randStatBase(_seed * 7);

		agility.scale = randFltWeighted1(_seed * 8, 1.55f, 1.45f);
		intelligence.scale = randFltWeighted1(_seed * 9, 1.55f, 1.45f);
		stamina.scale = randFltWeighted1(_seed * 10, 1.55f, 1.45f);
		vitality.scale = randFltWeighted1(_seed * 11, 1.55f, 1.45f);
		strength.scale = randFltWeighted1(_seed * 12, 1.55f, 1.45f);
		power.scale = randFltWeighted1(_seed * 13, 1.55f, 1.45f);

		abilities.push_back(0);// rand(_seed * 13) % abilityManager.getAbilityCount());
		abilities.push_back(1);// rand(_seed * 14) % abilityManager.getAbilityCount());
		abilities.push_back(2);
		defaultMove = 0;
	}
};

class CreatureManager
{
private:
	std::vector<Creature> creatures;
	CreatureAbilityManager abilityManager;

public:
	CreatureManager(GameData* data) : abilityManager(data)
	{}

	CreatureID newCreature(unsigned int creature);
	MeshAsset_const getCreatureMesh(CreatureID creature);
	float getCreatureScale(CreatureID creature);
	const Creature& getCreature(CreatureID creature);
	StatsStruct makeCreatureStats(CreatureID creature, unsigned int level);
	CreatureAbilityManager& getAbilityManager(){ return abilityManager; }
};