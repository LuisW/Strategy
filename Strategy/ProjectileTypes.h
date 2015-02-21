#pragma once

#include "CollisionSystem.h"

enum ProjectileType { PT_Bullet, PT_Wave };

struct IProjectile
{
public:
	void Move(float deltaT);
	void Collide(std::vector<CollDistData>& collisions, CollisionSystem& collSys)
};