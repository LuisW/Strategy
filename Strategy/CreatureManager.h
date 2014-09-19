#pragma once

#include <functional>
#include <vector>
#include "BehaviourManager.h"
#include "AssetManager.h"
#include "ei/glm/glm.hpp"
#include "IStatEffect.h"

typedef unsigned int CreatureID;

unsigned int rand(unsigned int seed);
unsigned int randuint(unsigned int seed, unsigned int min, unsigned max);
float randFltWeighted(unsigned int seed, float average, float maxDiff);
float randStatBase(unsigned int seed);

BehaviourID selectBehaviour(unsigned int _seed, BehaviourManager& behaviours, BehaviourType type);

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

	BehaviourID Idle;
	BehaviourID RangeSetup;

	Creature(unsigned int _seed, BehaviourManager& behaviours) : seed(_seed)
	{
		int r = randuint(seed, 0, 1000);
		perception = glm::pow((float)r / 500.0f - 1.0f, 3.0f);

		agility.baseValue = randStatBase(_seed * 2);
		intelligence.baseValue = randStatBase(_seed * 3);
		stamina.baseValue = randStatBase(_seed * 4);
		vitality.baseValue = randStatBase(_seed * 5);
		strength.baseValue = randStatBase(_seed * 6);
		power.baseValue = randStatBase(_seed * 7);

		agility.scale = randFltWeighted(_seed * 8, 1.55f, 1.45f);
		intelligence.scale = randFltWeighted(_seed * 9, 1.55f, 1.45f);
		stamina.scale = randFltWeighted(_seed * 10, 1.55f, 1.45f);
		vitality.scale = randFltWeighted(_seed * 11, 1.55f, 1.45f);
		strength.scale = randFltWeighted(_seed * 12, 1.55f, 1.45f);
		power.scale = randFltWeighted(_seed * 13, 1.55f, 1.45f);

		Idle = selectBehaviour(_seed * 14, behaviours, BT_Idle);
		RangeSetup = selectBehaviour(_seed * 15, behaviours, BT_RangedSetup);
	}
};

class CreatureManager
{
private:
	BehaviourManager behaviourManager;
	std::vector<Creature> creatures;

public:

	CreatureManager(){};
 
	BehaviourManager& getBehaviours()
	{
		return behaviourManager;
	}

	CreatureID newCreature(unsigned int creature);
	MeshAsset_const getCreatureMesh(CreatureID creature);
	float getCreatureScale(CreatureID creature);
	const Creature& getCreature(CreatureID creature);
	StatsStruct makeCreatureStats(CreatureID creature, unsigned int level);
};