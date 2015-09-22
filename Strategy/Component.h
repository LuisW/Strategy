#pragma once

#include <string>

enum ComponentType {CT_RENDER, CT_TRANSFORM, CT_CAMERA, CT_SCENEGRAPH, CT_VISIBILITY, CT_VELOCITY, CT_CREATUREAI,
					CT_STATS, CT_COLLISION, CT_PROJECTILE, CT_VITALS, CT_UNUSED};

typedef unsigned int EntityID;

class Component
{
private:
	ComponentType	m_type;
	EntityID		m_ownerID;
	std::string		m_name;

public:
	Component(ComponentType type, EntityID owner, const std::string& name) : m_type(type), m_ownerID(owner), m_name(name)
	{
	}

	inline ComponentType getType()
	{
		return m_type;
	}
	
	inline EntityID getOwner()
	{
		return m_ownerID;
	}

	inline const std::string& getName()
	{
		return m_name;
	}

	virtual ~Component()
	{}
};