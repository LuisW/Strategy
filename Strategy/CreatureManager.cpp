#include "CreatureManager.h"

unsigned int rand(unsigned int seed)
{
	seed = (seed ^ 61u) ^ (seed >> 16u);
	seed *= 9u;
	seed = seed ^ (seed >> 4u);
	seed *= 668265261u;
	return seed ^ (seed >> 15u);
}

unsigned int randuint(unsigned int seed, unsigned int min, unsigned max)
{
	seed = rand(seed);
	seed %= (max - min);
	return seed + min;
}

float randFltWeighted(unsigned int seed, float average, float maxDiff)
{
	unsigned int r = randuint(seed, 0, 10000);
	float f = glm::pow((float)r / 5000.0f - 1.0f, 3.0f);
	return f * maxDiff + average;
}

float randStatBase(unsigned int seed)
{
	unsigned int r = randuint(seed, 0, 30000);
	float x = (float)r / 10000.0f;
	return glm::pow(x - 1.5874f, 3.0f) + glm::pow(0.55874f * x, 6.0f) + 5;
}

BehaviourID selectBehaviour(unsigned int _seed, BehaviourManager& behaviours, BehaviourType type)
{
	return BehaviourID(type, rand(_seed) % (behaviours.getBehaviourCount(type)));
}

CreatureID CreatureManager::newCreature(unsigned int seed)
{
	for (unsigned int n = 0; n < creatures.size(); n++)
	{
		if (creatures[n].seed == seed)
		{
			return n;
		}
	}

	creatures.push_back(Creature(seed, behaviourManager));

	return creatures.size() - 1;
}

MeshAsset_const CreatureManager::getCreatureMesh(CreatureID creature)
{
	return AssetManager::getAsset<Mesh>(MeshKey("creature.tem"));
}

const Creature& CreatureManager::getCreature(CreatureID creature)
{
	return creatures[creature];
}

StatsStruct CreatureManager::makeCreatureStats(CreatureID creature, unsigned int level)
{
	StatsStruct stats;
	Creature cre = creatures[creature];

	stats.agility.current = stats.agility.standart = cre.agility.baseValue + cre.agility.scale * level;
	stats.intelligence.current = stats.intelligence.standart = cre.intelligence.baseValue + cre.intelligence.scale * level;
	stats.perception.current = stats.perception.standart = cre.perception;
	stats.power.current = stats.power.standart = cre.power.baseValue + cre.power.scale * level;
	stats.stamina.current = stats.stamina.standart = cre.stamina.baseValue + cre.stamina.scale * level;
	stats.strength.current = stats.strength.standart = cre.strength.baseValue + cre.strength.scale * level;
	stats.vitality.current = stats.vitality.standart = cre.vitality.baseValue + cre.vitality.scale * level;

	return stats;
}