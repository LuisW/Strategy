#pragma once

#include "System.h"
#include "CollisionFilters.h"
#include <vector>
#include <tuple>
#include "Ray.h"
#include "AssetManager.h"

struct CollDistData
{
	EntityID	m_id;
	float		m_sqrdist;

	bool operator<(const CollDistData& other)
	{
		return m_sqrdist < other.m_sqrdist;
	}

	CollDistData(EntityID id, float sqrdist) : m_id(id), m_sqrdist(sqrdist)
	{}
};

class CollisionSystem : public System
{
private:
	struct CollideableListEntry
	{
		bool		m_isStatic;
		EntityID	m_id;
		OBB			m_obb;

		CollideableListEntry(bool isStatic, EntityID id, const OBB& obb) : m_isStatic(isStatic), m_id(id), m_obb(obb)
		{

		}
	};

	std::vector<CollideableListEntry> m_entities;
	EntityManager& m_rEntityManager;
	float getTerrainPixel(int x, int z);

public:
	CollisionSystem(EntityManager& entityManager);

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);

	void PrepareFrame();

	void Collide(const OBB& box, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const glm::vec3& point, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const Ray& ray, bool positiveOnly, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const LineSegment line, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const LineSegment line, std::vector<CollDistData>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());

	void CollideTubeGWOpt(float smallR, float bigR, const glm::vec3& c, float height, std::vector<CollDistData>& collisions, ICollisionFilter& filter = DefaultCollisionFilter()); //Tube - OBB collision optimised for groundwaves (simplify cube to linesegment, assume tubes height is collinear to y-axis)

	void CollideNearest(const LineSegment line, EntityID& collision, ICollisionFilter& filter = DefaultCollisionFilter());

	float TerrainHeight(glm::vec2 position);
};