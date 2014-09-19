#include "CreatureAIComponent.h"
#include "CreatureManager.h"
#include "Behaviours.h"

CreatureAIComponent::CreatureAIComponent(EntityID owner, CreatureID _creatureType, CreatureManager& creatures, glm::vec3& location) : creatureType(_creatureType), mode(CAI_Idle), Component(CT_CreatureAI, owner, "")
{
	const Creature& creature = creatures.getCreatureStats(_creatureType);
	Idle = creatures.getBehaviours().getNewInstanceOf(creature.Idle, location);
	RangedSetup = creatures.getBehaviours().getNewInstanceOf(creature.RangeSetup, location);
}

CreatureAIComponent::~CreatureAIComponent()
{
	delete Idle;

	for (int n = 0; n < 4; n++)
	{
		delete Abilities[n];
	}

	delete RangedSetup;
	delete MeleeSetup;
}