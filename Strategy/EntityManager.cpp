#include "EntityManager.h"

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
		if (entities[entity].entity->hasComponent(component->getType()))
		{
			NotifyEntityChanged(entity, component->getType(), false);
			entities[entity].entity->AddComponent(component);
			NotifyEntityChanged(entity, component->getType(), true);
		}
		else
		{
			entities[entity].entity->AddComponent(component);
			NotifyEntityChanged(entity, component->getType(), true);
		}
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

EntityManager::~EntityManager()
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		delete entities[n].entity;
	}
}

//******************************************************************************************************
//****************************************  FactorySection  ********************************************
//******************************************************************************************************

EntityID EntityManager::newPlayer()
{
	EntityID ent = newEntity();
	entityAddComponent(ent, new CameraComponent(ent, "Cam", 60.0f, 4.0f/3.0f, 0.1f, 10000.0f, glm::vec3(819.3f, 0.0f, 819.3f), glm::angleAxis(0.0f,glm::vec3(0.0f,1.0f,0.0f))));
	entityAddComponent(ent, new RenderComponent(AssetManager::getAsset<Mesh>(MeshKey("M24r.tem")), ent, "M24r"));
	entityAddComponent(ent, new TransformComponent(ent, "Trans"));
	entityAddComponent(ent, new VelocityComponent(ent, glm::vec3()));
	entityAddComponent(ent, new StatsComponent(ent));
	return ent;
}

EntityID EntityManager::newTestObject()
{
	EntityID ent = newEntity();
	entityAddComponent(ent, new RenderComponent(AssetManager::getAsset<Mesh>(MeshKey("creature.tem")), ent, "TestMesh"));
	entityAddComponent(ent, new TransformComponent(ent, "TestTrans", glm::vec3(800.0f, 12.0f, 800.0f), glm::angleAxis(90.0f, glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f)));
	return ent;
}

EntityID EntityManager::newCreature(CreatureManager& creatures, unsigned int seed, glm::vec3& location)
{
	EntityID ent = newEntity();
	CreatureID creature = creatures.newCreature(seed);
	unsigned int level = rand(seed) % 100;
	entityAddComponent(ent, new RenderComponent(creatures.getCreatureMesh(creature), ent, "Creature"));
	entityAddComponent(ent, new TransformComponent(ent, "Creature", location, glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f)));
	entityAddComponent(ent, new CreatureAIComponent(ent, creature, creatures, location));
	entityAddComponent(ent, new CollisionComponent(ent, creatures.getCreatureMesh(creature).get().getBoundingVolume()));
	entityAddComponent(ent, new StatsComponent(ent, creatures.makeCreatureStats(creature)));

	return ent;
}