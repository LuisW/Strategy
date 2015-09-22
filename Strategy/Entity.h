#pragma once

#include "Component.h"
#include <vector>

class Entity
{
private:
	Component*	m_pComponents[CT_UNUSED];
	EntityID	m_id;

public:
	inline Entity()
	{}

	inline Entity(EntityID _id) : m_id(_id)
	{
		memset(m_pComponents, 0, sizeof(Component*) * CT_UNUSED);
	}

	inline EntityID getID()
	{
		return m_id;
	}

	inline void AddComponent(Component* comp)
	{
		if (m_pComponents[comp->getType()])
		{
			delete m_pComponents[comp->getType()];
		}
		
		m_pComponents[comp->getType()] = comp;
	}

	~Entity()
	{
		RemoveAllComponents();
	}

	inline void RemoveComponent(ComponentType type)
	{
		delete m_pComponents[type];
		m_pComponents[type] = NULL;
	}

	void RemoveAllComponents()
	{
		for (unsigned int n = 0; n < CT_UNUSED; n++)
		{
			RemoveComponent((ComponentType)n);
		}
	}

	inline bool hasComponent(ComponentType type)
	{
		return m_pComponents[type] != NULL;
	}

	inline Component* getComponent(ComponentType type)
	{
		return m_pComponents[type];
	}

	inline const Component* getComponent(ComponentType type) const
	{
		return m_pComponents[type];
	}
};