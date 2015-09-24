#include "EntityManager.h"
#include "AssetManager.h"

#define InvalidEntError(entity) ERROR_INFO(InvalidEntityMsg(entity).c_str()); throw;
#define InvalidCompError(type, entity) ERROR_INFO(InvalidComponentMsg(type, entity).c_str()); throw;

std::string EntityManager::InvalidEntityMsg(EntityID entity)
{
	std::string str("Invalid entity");

	if (entity)
	{
		str += ": Attempt to access invalid entity " + std::to_string(entity);
	}
	else
	{
		str += ": Attempt to access NULL entity";
	}

	return str;
}

std::string EntityManager::InvalidComponentMsg(ComponentType type, EntityID entity)
{
	std::string str("Invalid component");

	str += ": Attempt to access NULL component of type " + std::to_string(type) + " on entity " + std::to_string(entity);

	return str;
}

void EntityManager::NotifyEntityChanged(EntityID entity, ComponentType type, bool added)
{
	for (unsigned int n = 0; n < m_systems.size(); n++)
	{
		m_systems[n]->onEntityChanged(entity, type, added);
	}
}

void EntityManager::NotifyEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < m_systems.size(); n++)
	{
		m_systems[n]->onEntityRemoved(entity);
	}
}


EntityID EntityManager::newEntity()
{
	for (unsigned int n = 1; n < m_entities.size(); n++)
	{
		if (!m_entities[n].m_used)
		{
			m_entities[n].m_used = true;
			return n;
		}
	}

	m_entities.push_back(EntityListElem(new Entity(m_entities.size()), true));
	return m_entities.size() - 1;
}

bool EntityManager::entityHasComponent(EntityID entity, ComponentType type)
{
	if (m_entities[entity].m_used)
	{
		return m_entities[entity].m_pEntity->hasComponent(type);
	}
	else
	{
		InvalidEntError(entity);
	}
}

void EntityManager::entityAddComponent(EntityID entity, Component* component)
{
	if (m_entities[entity].m_used)
	{
		if (m_entities[entity].m_pEntity->hasComponent(component->getType()))
		{
			NotifyEntityChanged(entity, component->getType(), false);
			m_entities[entity].m_pEntity->AddComponent(component);
			NotifyEntityChanged(entity, component->getType(), true);
		}
		else
		{
			m_entities[entity].m_pEntity->AddComponent(component);
			NotifyEntityChanged(entity, component->getType(), true);
		}
	}
	else
	{
		InvalidEntError(entity);
	}
}

void EntityManager::entityRemoveComponent(EntityID entity, ComponentType type)
{
	if (m_entities[entity].m_used)
	{
		NotifyEntityChanged(entity, type, false);
		m_entities[entity].m_pEntity->RemoveComponent(type);
	}
	else
	{
		InvalidEntError(entity);
	}
}

void EntityManager::RegisterSystem(System* system)
{
	m_systems.push_back(system);
}

void EntityManager::UnregisterSystem(System* system)
{
	for (unsigned int n = 0; n < m_systems.size(); n++)
	{
		if (m_systems[n] == system)
		{
			m_systems.erase(m_systems.begin() + n);
		}
	}
}

void EntityManager::UnregisterAllSystems()
{
	m_systems.clear();
}

EntityManager::~EntityManager()
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		delete m_entities[n].m_pEntity;
	}
}

//******************************************************************************************************
//****************************************  FactorySection  ********************************************
//******************************************************************************************************

EntityID EntityManager::newPlayer()
{
	EntityID ent = newEntity();
	entityAddComponent(ent, new CameraComponent(ent, "Cam", 60.0f, 4.0f/3.0f, 0.1f, 10000.0f, glm::vec3(), glm::angleAxis(0.0f,glm::vec3(0.0f,1.0f,0.0f))));
	entityAddComponent(ent, new TransformComponent(ent, "Trans", glm::vec3(2.42654181, 0.000000000, -1.37973344), glm::quat(), glm::vec3(1.0f)));
	entityAddComponent(ent, new VelocityComponent(ent, glm::vec3()));
	entityAddComponent(ent, new CollisionComponent(ent, OBB(glm::vec4(1.0f,0.0f,0.0f,0.5f),
															glm::vec4(0.0f, 1.0f, 0.0f, 0.9f), 
															glm::vec4(0.0f, 0.0f, 1.0f, 0.15f),
															glm::vec3(0.0f, -0.9f, 0.0f))));
	return ent;
}

EntityID EntityManager::newTestObject()
{
	EntityID ent = newEntity();
	entityAddComponent(ent, new RenderComponent(AssetManager::getAsset<Mesh>(MeshKey("Cube.tem")), ent, "TestMesh"));
	entityAddComponent(ent, new TransformComponent(ent, "TestTrans", glm::vec3(0.0f, 0.0f, 3.0f), glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f)));
	ent = newEntity();
	entityAddComponent(ent, new RenderComponent(AssetManager::getAsset<Mesh>(MeshKey("Sphere.tem")), ent, "TestMesh"));
	entityAddComponent(ent, new TransformComponent(ent, "TestTrans", glm::vec3(0.0f, 0.0f, 0.0f), glm::angleAxis(0.0f, glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f)));
	return ent;
}

EntityID EntityManager::newStaticMesh(MeshAsset_const mesh, const Transform& trans)
{
	EntityID ent = newEntity();
	entityAddComponent(ent, new RenderComponent(mesh, ent, "mesh"));
	entityAddComponent(ent, new TransformComponent(ent, "TestTrans", trans));
	return ent;
}