#include "EntityManager.h"

#include "CameraComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "AssetManager.h"

void EntityManager::NotifyEntityChanged(EntityID entity, ComponentType type, bool added)
{
	for (unsigned int n = 0; n < systems.size(); n++)
	{
		systems[n]->onEntityChanged(entity, type, added);
	}
}

void EntityManager::NotifyEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < systems.size(); n++)
	{
		systems[n]->onEntityRemoved(entity);
	}
}


EntityID EntityManager::newEntity()
{
	for (unsigned int n = 1; n < entities.size(); n++)
	{
		if (!entities[n].used)
		{
			entities[n].used = true;
			return n;
		}
	}

	entities.push_back(EntityListElem(new Entity(entities.size()), true));
	return entities.size() - 1;
}

bool EntityManager::entityHasComponent(EntityID entity, ComponentType type)
{
	if (entities[entity].used)
	{
		return entities[entity].entity->hasComponent(type);
	}
	else
	{
		throw InvalidEntityException(entity);
	}
}

void EntityManager::entityAddComponent(EntityID entity, Component* component)
{
	if (entities[entity].used)
	{
		entities[entity].entity->AddComponent(component);
		NotifyEntityChanged(entity, component->getType(), true);
	}
	else
	{
		throw InvalidEntityException(entity);
	}
}

void EntityManager::entityRemoveComponent(EntityID entity, ComponentType type)
{
	if (entities[entity].used)
	{
		NotifyEntityChanged(entity, type, false);
		entities[entity].entity->RemoveComponent(type);
	}
	else
	{
		throw InvalidEntityException(entity);
	}
}

void EntityManager::RegisterSystem(System* system)
{
	systems.push_back(system);
}

void EntityManager::UnregisterSystem(System* system)
{
	for (unsigned int n = 0; n < systems.size(); n++)
	{
		if (systems[n] == system)
		{
			systems.erase(systems.begin() + n);
		}
	}
}

void EntityManager::UnregisterAllSystems()
{
	systems.clear();
}

EntityID EntityManager::newPlayer()
{
	EntityID ent = newEntity();
	entityAddComponent(ent, new CameraComponent(ent, "Cam", 60.0f, 4.0f/3.0f, 0.1f, 10000.0f, glm::vec3(0.0f, 9.0f, 0.0f), glm::angleAxis(0.0f,glm::vec3(0.0f,1.0f,0.0f))));
	entityAddComponent(ent, new VelocityComponent(ent, glm::vec3()));
	return ent;
}

EntityID EntityManager::newTestObject()
{
	EntityID ent = newEntity();
	entityAddComponent(ent, new RenderComponent(AssetManager::getAsset<Mesh>(MeshKey("forLuis.tem")), ent, "TestMesh"));
	entityAddComponent(ent, new TransformComponent(ent, "TestTrans", glm::vec3(8000.0f, 16.4f, 19651.0f), glm::angleAxis(90.0f, glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.1385f)));
	return ent;
}

EntityManager::~EntityManager()
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		delete entities[n].entity;
	}
}