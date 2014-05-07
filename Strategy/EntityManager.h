#pragma once

#include "Entity.h"
#include "System.h"
#include <stdexcept>

enum EntityBaseType {ET_Player};

class InvalidEntityException : public std::runtime_error
{
private:
	EntityID entity;

public:
	InvalidEntityException(EntityID _entity) : std::runtime_error("invalid entity"), entity(_entity)
	{}

	virtual const char* what() const throw()
	{
		std::string str(std::runtime_error::what());

		if (entity)
		{
			str += ": Attempt to access NULL entity";
		}
		else
		{
			str += ": Attempt to access invalid entity " + std::to_string(entity);
		}

		return str.c_str();
	}
};

class InvalidComponentException : public std::runtime_error
{
private:
	EntityID entity;
	ComponentType type;

public:
	InvalidComponentException(ComponentType _type, EntityID _entity) : std::runtime_error("invalid entity"), entity(_entity), type(_type)
	{}

	virtual const char* what() const throw()
	{
		std::string str(std::runtime_error::what());

		str += ": Attempt to access NULL component of type " + std::to_string(type) + " on entity " + std::to_string(entity);

		return str.c_str();
	}
};

class EntityManager
{
private:
	struct EntityListElem
	{
		Entity* entity;
		bool used;

		EntityListElem(Entity* _entity, bool _used) : entity(_entity), used(_used)
		{
		}
	};

	std::vector<EntityListElem> entities;
	std::vector<System*> systems;

	EntityManager(const EntityManager& other)
	{}

	const EntityManager& operator=(const EntityManager& rhs)
	{}

	void NotifyEntityChanged(EntityID entity, ComponentType type, bool added);
	void NotifyEntityRemoved(EntityID entity);

public:
	EntityManager()
	{
		entities.push_back(EntityListElem(NULL, false));
	}

	EntityID newEntity();

	inline void deleteEntity(EntityID entity)
	{
		entities[entity].entity->RemoveAllComponents();
		entities[entity].used = false;
		NotifyEntityRemoved(entity);
	}
	
	template<typename T>
	const T& entityGetComponent(EntityID entity) const
	{
		if (entities[entity].used)
		{
			T* comp = (T*)(entities[entity].entity->getComponent(T::getComponentType()));

			if (!comp)
			{
				throw InvalidComponentException(T::getComponentType(), entity);
			}

			return *comp;
		}
		else
		{
			throw InvalidEntityException(entity);
		}
	}

	template<typename T>
	T& entityGetComponent(EntityID entity)
	{
		if (entities[entity].used)
		{
			T* comp = (T*)(entities[entity].entity->getComponent(T::getComponentType()));

			if (!comp)
			{
				throw InvalidComponentException(T::getComponentType(), entity);
			}

			return *comp;
		}
		else
		{
			throw InvalidEntityException(entity);
		}
	}

	bool entityHasComponent(EntityID entity, ComponentType type);
	void entityAddComponent(EntityID entity, Component* component);
	void entityRemoveComponent(EntityID entity, ComponentType type);

	void RegisterSystem(System* system);
	void UnregisterSystem(System* system);
	void UnregisterAllSystems();

	~EntityManager();

	EntityID newPlayer();
	EntityID newTestObject();
};