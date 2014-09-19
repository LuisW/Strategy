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
	}

	static ComponentType getComponentType()
	{
		return CT_Vitals;
	}
};