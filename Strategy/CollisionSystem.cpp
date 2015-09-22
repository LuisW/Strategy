#include "CollisionSystem.h"
#include "CollisionComponent.h"
#include "DataStructs.h"
#include <algorithm>

CollisionSystem::CollisionSystem(EntityManager& _entityManager) : m_rEntityManager(_entityManager)
{
}

void CollisionSystem::onEntityChanged(EntityID entity, ComponentType type, bool added)
{
	if (type == CT_TRANSFORM && m_rEntityManager.entityHasComponent(entity, CT_COLLISION))
	{
		for (unsigned int n = 0; n < m_entities.size(); n++)
		{
			if (m_entities[n].m_id == entity)
			{
				m_entities[n].m_isStatic = !added;

				CollisionComponent& coll = m_rEntityManager.entityGetComponent<CollisionComponent>(entity);
				if (added)
				{
					Transform& trans = m_rEntityManager.entityGetComponent<TransformComponent>(entity);
					m_entities[n].m_obb = coll.getCollisionVolume().transform(trans);
				}
				else
				{
					m_entities[n].m_obb = coll.getCollisionVolume();
				}

				return;
			}
		}
	}
	else if (type == CT_COLLISION)
	{
		if (added)
		{
			CollisionComponent& coll = m_rEntityManager.entityGetComponent<CollisionComponent>(entity);
			if (m_rEntityManager.entityHasComponent(entity, CT_TRANSFORM))
			{
				Transform& trans = m_rEntityManager.entityGetComponent<TransformComponent>(entity);
				m_entities.push_back(CollideableListEntry(false, entity, coll.getCollisionVolume().transform(trans)));
			}
			else
			{
				m_entities.push_back(CollideableListEntry(true, entity, coll.getCollisionVolume()));
			}
		}
		else
		{
			for (unsigned int n = 0; n < m_entities.size(); n++)
			{
				if (m_entities[n].m_id == entity)
				{
					m_entities.erase(m_entities.begin() + n);
					return;
				}
			}
		}
	}
}

void CollisionSystem::onEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (m_entities[n].m_id == entity)
		{
			m_entities.erase(m_entities.begin() + n);
		}
	}
}

void CollisionSystem::PrepareFrame()
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (!m_entities[n].m_isStatic)
		{
			Transform& trans = m_rEntityManager.entityGetComponent<TransformComponent>(m_entities[n].m_id);
			CollisionComponent& coll = m_rEntityManager.entityGetComponent<CollisionComponent>(m_entities[n].m_id);

			m_entities[n].m_obb = coll.getCollisionVolume().transform(trans);
		}
	}
}

void CollisionSystem::Collide(const OBB& box, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (filter.Filter(m_entities[n].m_id, m_rEntityManager))
			continue;

		//bounding sphere pre-test 

		glm::vec3 distv = box.getCenter() - m_entities[n].m_obb.getCenter();
		float dist = glm::dot(distv, distv);
		float dist2 = box.getRadius() + m_entities[n].m_obb.getRadius();
		dist2 *= dist2;

		if (dist > dist2)
		{
			continue;
		}

		if (box.IntersectOBB(m_entities[n].m_obb) == IT_Intersect)
		{
			collisions.push_back(m_entities[n].m_id);
		}
	}
}

void CollisionSystem::Collide(const glm::vec3& point, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (filter.Filter(m_entities[n].m_id, m_rEntityManager) && m_entities[n].m_obb.IntersectPoint(point) == IT_Inside)
		{
			collisions.push_back(m_entities[n].m_id);
		}
	}
}

void CollisionSystem::Collide(const Ray& ray, bool frontOnly, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (filter.Filter(m_entities[n].m_id, m_rEntityManager) && m_entities[n].m_obb.IntersectRay(ray, true) == IT_Intersect)
		{
			collisions.push_back(m_entities[n].m_id);
		}
	}
}

void CollisionSystem::Collide(const LineSegment line, std::vector<EntityID>& collisions, ICollisionFilter& filter)
{
	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (filter.Filter(m_entities[n].m_id, m_rEntityManager) && m_entities[n].m_obb.IntersectLineSegment(line) == IT_Intersect)
		{
			collisions.push_back(m_entities[n].m_id);
		}
	}
}

void CollisionSystem::Collide(const LineSegment line, std::vector<CollDistData>& collisions, ICollisionFilter& filter)
{
	float sqrdist = 0.0f;

	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (filter.Filter(m_entities[n].m_id, m_rEntityManager) && m_entities[n].m_obb.IntersectLineSegment(line, sqrdist) == IT_Intersect)
		{
			collisions.push_back(CollDistData(m_entities[n].m_id, sqrdist));
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

	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (!filter.Filter(m_entities[n].m_id, m_rEntityManager))
			continue;

		o2 = m_entities[n].m_obb.getCenter() - c;
		r = glm::vec3(m_entities[n].m_obb.getV()) * m_entities[n].m_obb.getV().w;

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

		collisions.push_back(CollDistData(m_entities[n].m_id, sqrdist));
	}

	std::sort(collisions.begin(), collisions.end());
}

void CollisionSystem::CollideNearest(const LineSegment line, EntityID& collision, ICollisionFilter& filter)
{
	collision = NULL;
	float min = INFINITY;
	float sqrdist = 0.0f;

	for (unsigned int n = 0; n < m_entities.size(); n++)
	{
		if (filter.Filter(m_entities[n].m_id, m_rEntityManager) && m_entities[n].m_obb.IntersectLineSegment(line, sqrdist) == IT_Intersect)
		{
			if (sqrdist < min)
			{
				min = sqrdist;
				collision = m_entities[n].m_id;
			}
		}
	}
}

//float CollisionSystem::getTerrainPixel(int x, int z)
//{
//	x = x % hmap.get().getWidth();
//	z = z % hmap.get().getWidth();
//
//	int bpp = hmap.get().getImage()->format->BytesPerPixel;
//	unsigned short *p = (unsigned short*)((unsigned char*)hmap.get().getImage()->pixels + z * hmap.get().getImage()->pitch + x * bpp);
//
//	return (p[0] / (float)((1<<8*bpp) - 1)) * TerrainSettings::Yscale;
//}

float CollisionSystem::TerrainHeight(glm::vec2 pos)
{
	return TerrainSettings::Yscale * 0.7f;

	//pos -= 2.0f;

	//float cx = TerrainSettings::mapScale / (float)(hmap.get().getWidth() - 1);
	//float cz = TerrainSettings::mapScale / (float)(hmap.get().getHeight() - 1);

	//pos.x /= cx;
	//pos.y /= cz;

	//if (glm::fract(pos.x) > glm::fract(pos.y))
	//{
	//	float l1 = (1.0f - glm::fract(pos.x));
	//	float l2 = glm::fract(pos.y);
	//	float l3 = 1.0f - l1 - l2;

	//	return l1 * getTerrainPixel((int)glm::floor(pos.x), (int)glm::floor(pos.y)) +
	//		   l3 * getTerrainPixel((int)glm::ceil(pos.x), (int)glm::floor(pos.y)) +
	//		   l2 * getTerrainPixel((int)glm::ceil(pos.x), (int)glm::ceil(pos.y));
	//}
	//else
	//{
	//	float l1 = (1.0f - glm::fract(pos.y));
	//	float l2 = glm::fract(pos.x);
	//	float l3 = 1.0f - l1 - l2;

	//	return l1 * getTerrainPixel((int)glm::floor(pos.x), (int)glm::floor(pos.y)) +
	//		   l3 * getTerrainPixel((int)glm::floor(pos.x), (int)glm::ceil(pos.y)) +
	//		   l2 * getTerrainPixel((int)glm::ceil(pos.x), (int)glm::ceil(pos.y));
	//}
}