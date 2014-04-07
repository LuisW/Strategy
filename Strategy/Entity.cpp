#include "Entity.h"


void Entity::RemoveComponent(ComponentType type)
{
	for (unsigned int n = 0; n < Components.size(); n++)
	{
		if (Components[n]->getType() == type)
		{
			delete Components[n];
			Components.erase(Components.begin() + n);
			return;
		}
	}
}

void Entity::RemoveComponent(ComponentType type, const std::string& name)
{
	for (unsigned int n = 0; n < Components.size(); n++)
	{
		if (Components[n]->getType() == type && Components[n]->getName() == name)
		{
			delete Components[n];
			Components.erase(Components.begin() + n);
			return;
		}
	}
}

bool Entity::hasComponent(ComponentType type)
{
	for (unsigned int n = 0; n < Components.size(); n++)
	{
		if (Components[n]->getType() == type)
		{
			return true;
		}
	}

	return false;
}

bool Entity::hasComponent(const std::string& name)
{
	for (unsigned int n = 0; n < Components.size(); n++)
	{
		if (Components[n]->getName() == name)
		{
			return true;
		}
	}

	return false;
}

Component& Entity::getComponent(ComponentType type)
{
	for (unsigned int n = 0; n < Components.size(); n++)
	{
		if (Components[n]->getType() == type)
		{
			return *Components[n];
		}
	}
}

Component& Entity::getComponent(const std::string& name)
{
	for (unsigned int n = 0; n < Components.size(); n++)
	{
		if (Components[n]->getName() == name)
		{
			return *Components[n];
		}
	}
}