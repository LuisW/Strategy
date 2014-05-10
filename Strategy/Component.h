#pragma once

#include <string>

enum ComponentType {CT_Render, CT_Transform, CT_Camera, CT_SceneGraph, CT_Visibility, CT_Unused};

typedef unsigned int EntityID;

class Component
{
private:
	ComponentType type;
	EntityID ownerID;
	std::string name;

public:
	Component(ComponentType _type, EntityID owner, const std::string& _name) : type(_type), ownerID(owner), name(_name)
	{
	}

	inline ComponentType getType()
	{
		return type;
	}
	
	inline EntityID getOwner()
	{
		return ownerID;
	}

	inline const std::string& getName()
	{
		return name;
	}

	~Component()
	{}
};