#include "StatSystem.h"
#include "EntityManager.h"
#include "StatsComponent.h"
#include "VitalsComponent.h"

StatSystem::StatSystem(EntityManager& _entityManager) : entityManager(_entityManager)
{
}

void StatSystem::onEntityChanged(EntityID entity, ComponentType type, bool added)
{
	if (type != CT_Stats)
		return;

	if (added)
	{
		entities.push_back(entity);
	}
	else
	{
		for (unsigned int n = 0; n < entities.size(); n++)
		{
			if (entities[n] == entity)
			{
				entities.erase(entities.begin() + n);
			}
		}
	}
}

void StatSystem::onEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (entities[n] == entity)
		{
			entities.erase(entities.begin() + n);
		}
	}
}

void StatSystem::Tick(double deltaT)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		StatsComponent& statscmp = entityManager.entityGetComponent<StatsComponent>(entities[n]);

		StatsStruct& stats = statscmp;

		stats.agility.current = stats.agility.standart;
		stats.intelligence.current = stats.intelligence.standart;
		stats.perception.current = stats.perception.standart;
		stats.power.current = stats.power.standart;
		stats.stamina.current = stats.stamina.standart;
		stats.strength.current = stats.strength.standart;
		stats.vitality.current = stats.vitality.standart;

		statscmp.ResetIterator();

		while (statscmp.ValidIterator())
		{
			StatEffectID effect = statscmp.getNext();

			if (statscmp.getEffect(effect).hasEnded() || statscmp.getEffect(effect).statEffect(stats, entities[n], deltaT))
			{
				statscmp.RemoveEffect(effect);
			}
		}

		stats.agility.current = glm::max(0.0f, stats.agility.current);
		stats.intelligence.current = glm::max(0.0f, stats.intelligence.current);
		stats.perception.current = glm::max(0.0f, stats.perception.current);
		stats.power.current = glm::max(0.0f, stats.power.current);
		stats.stamina.current = glm::max(0.0f, stats.stamina.current);
		stats.strength.current = glm::max(0.0f, stats.strength.current);
		stats.vitality.current = glm::max(0.0f, stats.vitality.current);

		if (entityManager.entityHasComponent(entities[n], CT_Vitals))
		{
			VitalsComponent& vitals = entityManager.entityGetComponent<VitalsComponent>(entities[n]);

			vitals.maxHealth = 100.0f * stats.vitality.current;
			vitals.maxMana = 10.0f * stats.power.current;

			float regenPerSec = stats.vitality.current * 0.5f + stats.stamina.current * (100.0f / 330.0f);
			vitals.healthPercent += (regenPerSec / vitals.maxHealth) * (float)deltaT;
			vitals.healthPercent = glm::min(vitals.healthPercent, 1.0f);

			regenPerSec = stats.power.current * 0.02f + stats.stamina.current * (15.0f / 330.0f);
			vitals.manaPercent += (regenPerSec / vitals.maxMana) * (float)deltaT;
			vitals.manaPercent = glm::min(vitals.manaPercent, 1.0f);
		}

		stats.derived.movementspeed = stats.agility.current * (10.0f / 300.0f);
		stats.derived.turnspeed = (stats.agility.current * (3.1415f / 500.0f)) / glm::max(1.0f, stats.vitality.current * 0.005f);

		statscmp.ResetIterator();

		while (statscmp.ValidIterator())
		{
			StatEffectID effect = statscmp.getNext();

			if (statscmp.getEffect(effect).hasEnded() || statscmp.getEffect(effect).derivedEffect(stats, entities[n], deltaT))
			{
				statscmp.RemoveEffect(effect);
			}
		}
	}
}
