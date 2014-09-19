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
		
		statscmp.ResetIterator();

		while (statscmp.ValidIterator())
		{
			StatEffectID effect = statscmp.getNext();

			if (statscmp.getEffect(effect).hasEnded() || statscmp.getEffect(effect).effect(entities[n], deltaT))
			{
				statscmp.RemoveEffect(effect);
			}
		}

		if (entityManager.entityHasComponent(entities[n], CT_Vitals))
		{
			VitalsComponent& vitals = entityManager.entityGetComponent<VitalsComponent>(entities[n]);

			vitals.maxHealth = stats.vitality.current;
		}

		stats.derived.movementspeed = stats.agility.current;
	}
}
