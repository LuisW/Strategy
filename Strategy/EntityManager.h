#pragma once

#include "Entity.h"
#include "System.h"
#include <stdexcept>

#include "CameraComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "CollisionComponent.h"
#include "DebugHelp.h"

class IProjectileEffect;

#define InvalidEntError(entity) ERROR_INFO(InvalidEntityMsg(entity).c_str()); throw;
#define InvalidCompError(type, entity) ERROR_INFO(InvalidComponentMsg(type, entity).c_str()); throw;

class EntityManager
{
private:
	struct EntityListElem
	{
		Entity* m_pEntity;
		bool	m_used;

		EntityListElem(Entity* entity, bool used) : m_pEntity(entity), m_used(used)
		{
		}
	};

	std::vector<EntityListElem> m_entities;
	std::vector<System*>		m_systems;

	EntityManager(const EntityManager& other)
	{}

	const EntityManager& operator=(const EntityManager& rhs)
	{}

	void NotifyEntityChanged(EntityID entity, ComponentType type, bool added);
	void NotifyEntityRemoved(EntityID entity);

	std::string InvalidEntityMsg(EntityID entity);
	std::string InvalidComponentMsg(ComponentType type, EntityID entity);

public:
	EntityManager()
	{
		m_entities.push_back(EntityListElem(NULL, false));
	}

	EntityID newEntity();

	inline void deleteEntity(EntityID entity)
	{
		NotifyEntityRemoved(entity);
		m_entities[entity].m_pEntity->RemoveAllComponents();
		m_entities[entity].m_used = false;
	}
	
	template<typename T>
	const T& entityGetComponent(EntityID entity) const
	{
		if (m_entities[entity].m_used)
		{
			T* comp = (T*)(m_entities[entity].m_pEntity->getComponent(T::getComponentType()));

			if (!comp)
			{
				InvalidCompError(T::getComponentType(), entity)
			}

			return *comp;
		}
		else
		{
			InvalidEntError(entity);
		}
	}

	template<typename T>
	T& entityGetComponent(EntityID entity)
	{
		if (m_entities[entity].m_used)
		{
			T* comp = (T*)(m_entities[entity].m_pEntity->getComponent(T::getComponentType()));

			if (!comp)
			{
				InvalidCompError(T::getComponentType(), entity)
			}

			return *comp;
		}
		else
		{
			InvalidEntError(entity)
		}
	}

	bool entityHasComponent(EntityID entity, ComponentType type);
	void entityAddComponent(EntityID entity, Component* component);
	void entityRemoveComponent(EntityID entity, ComponentType type);

	void RegisterSystem(System* system);
	void UnregisterSystem(System* system);
	void UnregisterAllSystems();

	~EntityManager();

//************FactorySection*************

public:
	EntityID newPlayer();
	EntityID newTestObject();
	EntityID newStaticMesh(MeshAsset_const mesh, const Transform& trans);
	EntityID newBullet(EntityID source, glm::vec3 pos, glm::vec3 dir, IProjectileEffect* effect);
};

#undef InvalidEntError
#undef InvalidCompError