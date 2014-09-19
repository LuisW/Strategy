#pragma once

#include "Component.h"
#include "ei/glm/glm.hpp"
#include "CreatureManager.h"

enum CreatureAIMode { CAI_Idle, CAI_Combat, CAI_Search };

class CreatureAIComponent : public Component
{
private:
	CreatureID creatureType;
	CreatureAIMode mode;
	long combatTimer;

	IBehaviour* Idle;
	IAbility* Abilities[4];
	IBehaviour* RangedSetup;
	IBehaviour* MeleeSetup;

public:

	CreatureAIComponent(EntityID owner, CreatureID _creatureType, CreatureManager& creatures, glm::vec3& location);

	inline void setCombatTimer(long _combatTimer)
	{
		combatTimer = _combatTimer;
	}

	inline long getCombatTimer()
	{
		return combatTimer;
	}

	inline CreatureID getCreatureType()
	{
		return creatureType;
	}

	inline CreatureAIMode getMode()
	{
		return mode;
	}

	inline void setMode(CreatureAIMode _mode)
	{
		mode = _mode;
	}

	inline IBehaviour& getIdle()
	{
		return *Idle;
	}

	inline IBehaviour& getRangedSetup()
	{
		return *RangedSetup;
	}

	static ComponentType getComponentType()
	{
		return CT_CreatureAI;
	}

	~CreatureAIComponent();
};