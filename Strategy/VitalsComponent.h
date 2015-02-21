#pragma once

#include "Component.h"

struct VitalsStruct
{
	float healthPercent;
	float manaPercent;
	float maxHealth;
	float maxMana;
};

class VitalsComponent : public Component, public VitalsStruct
{
private:

public:
	VitalsComponent(EntityID owner) : Component(CT_Vitals, owner, "")
	{
		healthPercent = 1.0f;
		manaPercent = 1.0f;
	}

	static ComponentType getComponentType()
	{
		return CT_Vitals;
	}
};