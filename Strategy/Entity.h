#pragma once

#include "Component.h"
#include <vector>

class Entity
{
private:
	Component* Components[CT_Unused];
	EntityID id;

public:
	inline Entity()
	{}

	inline Entity(EntityID _id) : id(_id)
	{
		memset(Components, 0, sizeof(Component*) * CT_Unused);
	}

	inline EntityID getID()
	{
		return id;
	}

	inline void AddComponent(Component* comp)
	{
		if (Components[comp->getType()])
		{
			delete Components[comp->getType()];
		}
		
		Components[comp->getType()] = comp;
	}

	~Entity()
	{
		RemoveAllComponents();
	}

	inline void RemoveComponent(ComponentType type)
	{
		delete Components[type];
		Components[type] = NULL;
	}

	void RemoveAllComponents()
	{
		for (unsigned int n = 0; n < CT_Unused; n++)
		{
			RemoveComponent((ComponentType)n);
		}
	}

	inline bool hasComponent(ComponentType type)
	{
		return Components[type] != NULL;
	}

	inline Component* getComponent(ComponentType type)
	{
		return Components[type];
	}

	inline const Component* getComponent(ComponentType type) const
	{
		return Components[type];
	}
};