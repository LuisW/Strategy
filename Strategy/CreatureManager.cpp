#include "CreatureManager.h"

CreatureID CreatureManager::newCreature(unsigned int seed)
{
	for (unsigned int n = 0; n < creatures.size(); n++)
	{
		if (creatures[n].seed == seed)
		{
			return n;
		}
	}

	creatures.push_back(Creature(seed, abilityManager));

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