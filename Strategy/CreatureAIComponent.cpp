#include "CreatureAIComponent.h"
#include "CreatureManager.h"

CreatureAIComponent::CreatureAIComponent(EntityID owner, CreatureID _creatureType, CreatureManager& creatures, glm::vec3& location) : creatureType(_creatureType), Component(CT_CreatureAI, owner, "")
{
	const Creature& creature = creatures.getCreature(_creatureType);
	for (unsigned int n = 0; n < creature.abilities.size(); n++)
	{
		abilities.push_back(creatures.getAbilityManager().makeNewInstance(creature.abilities[n], owner));
	}

	moveAbility = creatures.getAbilityManager().makeNewBaseMoveInstance(creature.defaultMove, owner);
}

CreatureAIComponent::~CreatureAIComponent()
{
	for (int n = 0; n < abilities.size(); n++)
	{
		delete abilities[n];
	}
	delete moveAbility;
}