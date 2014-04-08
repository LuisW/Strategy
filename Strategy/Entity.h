#pragma once

#include "Component.h"
#include <vector>

class Entity
{
private:
	std::vector<Component*> Components;
	EntityID id;

public:
	inline Entity()
	{}

	inline Entity(EntityID _id) : id(_id)
	{
	}

	inline EntityID getID()
	{
		return id;
	}

	inline void AddComponent(Component* comp)
	{
		Components.push_back(comp);
	}

	void RemoveComponent(ComponentType type);
	void RemoveComponent(ComponentType type, const std::string& name);
	void RemoveAllComponents();
	bool hasComponent(ComponentType type);
	bool hasComponent(const std::string& name);
	Component& getComponent(ComponentType type);
	Component& getComponent(const std::string& name);
};