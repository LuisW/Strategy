#pragma once

#include "System.h"
#include "EntityManager.h"
#include <vector>
#include "Ray.h"

class ICollisionFilter
{
private:
public:
	virtual bool Filter(EntityID ent) = 0;
	virtual ~ICollisionFilter(){};
};

class DefaultCollisionFilter : public ICollisionFilter
{
private:
public:
	bool Filter(EntityID ent)
	{
		return true;
	}
};

struct CollDistData
{
	EntityID id;
	float sqrdist;

	bool operator<(const CollDistData& other)
	{
		return sqrdist < other.sqrdist;
	}

	CollDistData(EntityID _id, float _sqrdist) : id(_id), sqrdist(_sqrdist)
	{}
};

class CollisionSystem : public System
{
private:
	struct CollideableListEntry
	{
		bool isStatic;
		EntityID id;
		OBB obb;

		CollideableListEntry(bool _isStatic, EntityID _id, const OBB& _obb) : isStatic(_isStatic), id(_id), obb(_obb)
		{

		}
	};

	std::vector<CollideableListEntry> entities;
	EntityManager& entityManager;
	TextureAsset_const hmap;
	float getTerrainPixel(int x, int z);

public:
	CollisionSystem(EntityManager& _entityManager);

	void onEntityChanged(EntityID entity, ComponentType type, bool added);
	void onEntityRemoved(EntityID entity);

	void PrepareFrame();

	void Collide(const OBB& box, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const glm::vec3& point, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const Ray& ray, bool positiveOnly, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const LineSegment line, std::vector<EntityID>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void Collide(const LineSegment line, std::vector<CollDistData>& collisions, ICollisionFilter& filter = DefaultCollisionFilter());
	void CollideNearest(const LineSegment line, EntityID& collision, ICollisionFilter& filter = DefaultCollisionFilter());

	float TerrainHeight(glm::vec2 position);
};