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
		if (filter.Filter(entities[n].id, entityManager))
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
		if (filter.Filter(entities[n].id, entityManager) && entities[n].obb.IntersectPoint(point) == IT_Inside)
		{
			collisions.push_back(entities[n].id);
		}
	}
}

void CollisionSystem::Collide(const Ray& ray, bool frontOnly, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id, entityManager) && entities[n].obb.IntersectRay(ray, true) == IT_Intersect)
		{
			collisions.push_back(entities[n].id);
		}
	}
}

void CollisionSystem::Collide(const LineSegment line, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (filter.Filter(entities[n].id, entityManager) && entities[n].obb.IntersectLineSegment(line) == IT_Intersect)
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
		if (filter.Filter(entities[n].id, entityManager) && entities[n].obb.IntersectLineSegment(line, sqrdist) == IT_Intersect)
		{
			collisions.push_back(CollDistData(entities[n].id, sqrdist));
		}
	}

	std::sort(collisions.begin(), collisions.end());
}

float sqr(float x)
{
	return x*x;
}

void CollisionSystem::CollideTubeGWOpt(float smallR, float bigR, const glm::vec3& c, float height, std::vector<CollDistData>& collisions, ICollisionFilter& filter)
//Tube - OBB collision optimised for groundwaves (simplify cube to linesegment, assume tubes height is collinear to y-axis)
{
	float sqrdist = 0.0f;

	height *= 0.5f;
	bigR *= bigR;
	smallR *= smallR;

	glm::vec3 r;
	glm::vec3 o2;
	glm::vec2 d;
	glm::vec2 t;

	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (!filter.Filter(entities[n].id, entityManager))
			continue;

		o2 = entities[n].obb.getCenter() - c;
		r = glm::vec3(entities[n].obb.getV()) * entities[n].obb.getV().w;

		if (glm::abs(r.y) < 1e-06f)
		{
			if (glm::abs(o2.y) <= height)
			{
				t = glm::vec2(-1.0f, 1.0f);
			}
			else
			{
				continue;
			}
		}
		else
		{
			
			if (r.y < 0.0f)
			{
				t = glm::vec2((height - o2.y) / r.y, (-height - o2.y) / r.y);
			}
			else
			{
				t = glm::vec2((-height - o2.y) / r.y, (height - o2.y) / r.y);
			}
		}

		t.x = glm::max(t.x, -1.0f);
		t.y = glm::min(t.y, 1.0f);

		if (t.x > t.y)
			continue;

		if (r.x < 1e-06f && r.z < 1e-06f)
		{
			float rad = o2.x * o2.x + o2.z * o2.z;

			if (rad < smallR || rad > bigR)
				continue;

			sqrdist = rad;
		}
		else
		{
			float tmin = -(o2.x*r.x + o2.z*r.z) / (r.x*r.x + r.z*r.z);

			if (t.x < tmin && 0.5f*(t.x + t.y) > tmin)
			{
				d.x = sqr(o2.x + r.x * tmin) + sqr(o2.z + r.z * tmin);
				d.y = sqr(o2.x + r.x * t.x) + sqr(o2.z + r.z * t.x);
			}
			else if (t.y > tmin && 0.5f*(t.x + t.y) < tmin)
			{
				d.x = sqr(o2.x + r.x * tmin) + sqr(o2.z + r.z * tmin);
				d.y = sqr(o2.x + r.x * t.y) + sqr(o2.z + r.z * t.y);
			}
			else
			{
				d.x = sqr(o2.x + r.x * t.x) + sqr(o2.z + r.z * t.x);
				d.y = sqr(o2.x + r.x * t.y) + sqr(o2.z + r.z * t.y);

				if (d.x > d.y)
				{
					float tmp = d.x;
					d.x = d.y;
					d.y = tmp;
				}
			}

			t.x = glm::max(smallR, d.x);
			t.y = glm::min(bigR, d.y);

			if (t.x > t.y)
				continue;

			sqrdist = glm::max(d.x, smallR);
		}

		collisions.push_back(CollDistData(entities[n].id, sqrdist));
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
		if (filter.Filter(entities[n].id, entityManager) && entities[n].obb.IntersectLineSegment(line, sqrdist) == IT_Intersect)
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
	x = x % hmap.get().getWidth();
	z = z % hmap.get().getWidth();

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