#include "CollisionSystem.h"
#include "CollisionComponent.h"
#include "DataStructs.h"
#include <algorithm>

CollisionSystem::CollisionSystem(EntityManager& _entityManager) : entityManager(_entityManager),
	hmap(AssetManager::getAsset<Texture>(TextureKey("heightmap.png")))
{
}

void CollisionSystem::onEntityChanged(EntityID entity, ComponentType type, bool added)
{
	if (type == CT_Transform && entityManager.entityHasComponent(entity, CT_Collision))
	{
		for (unsigned int n = 0; n < entities.size(); n++)
		{
			if (entities[n].id == entity)
			{
				entities[n].isStatic = !added;

				CollisionComponent& coll = entityManager.entityGetComponent<CollisionComponent>(entity);
				if (added)
				{
					Transform& trans = entityManager.entityGetComponent<TransformComponent>(entity);
					entities[n].obb = coll.getCollisionVolume().transform(trans);
				}
				else
				{
					entities[n].obb = coll.getCollisionVolume();
				}

				return;
			}
		}
	}
	else if (type == CT_Collision)
	{
		if (added)
		{
			CollisionComponent& coll = entityManager.entityGetComponent<CollisionComponent>(entity);
			if (entityManager.entityHasComponent(entity, CT_Transform))
			{
				Transform& trans = entityManager.entityGetComponent<TransformComponent>(entity);
				entities.push_back(CollideableListEntry(false, entity, coll.getCollisionVolume().transform(trans)));
			}
			else
			{
				entities.push_back(CollideableListEntry(true, entity, coll.getCollisionVolume()));
			}
		}
		else
		{
			for (unsigned int n = 0; n < entities.size(); n++)
			{
				if (entities[n].id == entity)
				{
					entities.erase(entities.begin() + n);
					return;
				}
			}
		}
	}
}

void CollisionSystem::onEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (entities[n].id == entity)
		{
			entities.erase(entities.begin() + n);
		}
	}
}

void CollisionSystem::PrepareFrame()
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (!entities[n].isStatic)
		{
			Transform& trans = entityManager.entityGetComponent<TransformComponent>(entities[n].id);
			CollisionComponent& coll = entityManager.entityGetComponent<CollisionComponent>(entities[n].id);

			entities[n].obb = coll.getCollisionVolume().transform(trans);
		}
	}
}

void CollisionSystem::Collide(const OBB& box, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id))
			continue;

		//bounding sphere pre-test 

		glm::vec3 distv = box.getCenter() - entities[n].obb.getCenter();
		float dist = glm::dot(distv, distv);
		float dist2 = box.getRadius() + entities[n].obb.getRadius();
		dist2 *= dist2;

		if (dist > dist2)
		{
			continue;
		}

		if (box.IntersectOBB(entities[n].obb) == IT_Intersect)
		{
			collisions.push_back(entities[n].id);
		}
	}
}

void CollisionSystem::Collide(const glm::vec3& point, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id) && entities[n].obb.IntersectPoint(point) == IT_Inside)
		{
			collisions.push_back(entities[n].id);
		}
	}
}

void CollisionSystem::Collide(const Ray& ray, bool frontOnly, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id) && entities[n].obb.IntersectRay(ray, true) == IT_Intersect)
		{
			collisions.push_back(entities[n].id);
		}
	}
}

void CollisionSystem::Collide(const LineSegment line, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id) && entities[n].obb.IntersectLineSegment(line) == IT_Intersect)
		{
			collisions.push_back(entities[n].id);
		}
	}
}

void CollisionSystem::Collide(const LineSegment line, std::vector<CollDistData>& collisions, ICollisionFilter& filter)
{
	float sqrdist = 0.0f;

	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id) && entities[n].obb.IntersectLineSegment(line, sqrdist) == IT_Intersect)
		{
			collisions.push_back(CollDistData(entities[n].id, sqrdist));
		}
	}

	std::sort(collisions.begin(), collisions.end());
}

void CollisionSystem::CollideNearest(const LineSegment line, EntityID& collision, ICollisionFilter& filter)
{
	collision = NULL;
	float min = INFINITY;
	float sqrdist = 0.0f;

	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id) && entities[n].obb.IntersectLineSegment(line, sqrdist) == IT_Intersect)
		{
			if (sqrdist < min)
			{
				min = sqrdist;
				collision = entities[n].id;
			}
		}
	}
}

float CollisionSystem::getTerrainPixel(int x, int z)
{
	int bpp = hmap.get().getImage()->format->BytesPerPixel;
	unsigned char *p = (unsigned char *)hmap.get().getImage()->pixels + z * hmap.get().getImage()->pitch + x * bpp;

	return (p[0] / 255.0f) * TerrainSettings::Yscale;
}

float CollisionSystem::TerrainHeight(glm::vec2 pos)
{
	pos -= 2.0f;

	float cx = TerrainSettings::mapScale / (float)(hmap.get().getWidth() - 1);
	float cz = TerrainSettings::mapScale / (float)(hmap.get().getHeight() - 1);

	pos.x /= cx;
	pos.y /= cz;

	if (glm::fract(pos.x) > glm::fract(pos.y))
	{
		float l1 = (1.0f - glm::fract(pos.x));
		float l2 = glm::fract(pos.y);
		float l3 = 1.0f - l1 - l2;

		return l1 * getTerrainPixel((int)glm::floor(pos.x), (int)glm::floor(pos.y)) +
			   l3 * getTerrainPixel((int)glm::ceil(pos.x), (int)glm::floor(pos.y)) +
			   l2 * getTerrainPixel((int)glm::ceil(pos.x), (int)glm::ceil(pos.y));
	}
	else
	{
		float l1 = (1.0f - glm::fract(pos.y));
		float l2 = glm::fract(pos.x);
		float l3 = 1.0f - l1 - l2;

		return l1 * getTerrainPixel((int)glm::floor(pos.x), (int)glm::floor(pos.y)) +
			   l3 * getTerrainPixel((int)glm::floor(pos.x), (int)glm::ceil(pos.y)) +
			   l2 * getTerrainPixel((int)glm::ceil(pos.x), (int)glm::ceil(pos.y));
	}
}