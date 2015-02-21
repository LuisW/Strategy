#include "ProjectileSystem.h"

ProjectileSystem::ProjectileSystem(EntityManager& _entityManager, CollisionSystem& _collisionSystem)
	: entityManager(_entityManager), collisionSystem(_collisionSystem)
{

}

void ProjectileSystem::onEntityChanged(EntityID entity, ComponentType type, bool added)
{
	if (type != CT_Velocity && type != CT_Projectile)
		return;

	if (added)
	{
		if (entityManager.entityHasComponent(entity, CT_Projectile))
		{
			ProjectileComponent& projectile = entityManager.entityGetComponent<ProjectileComponent>(entity);
			if (entityManager.entityHasComponent(entity, CT_Velocity) || projectile.getProjectileType() == PT_GroundWave)
				ents[projectile.getProjectileType()].push_back(entity);
		}
	}
	else
	{
		for (unsigned int i = 0; i < PT_Unused; i++)
		{
			for (unsigned int n = 0; n < ents[i].size(); n++)
			{
				if (ents[i][n] == entity)
				{
					ents[i].erase(ents[i].begin() + n);
				}
			}
		}
	}
}

void ProjectileSystem::onEntityRemoved(EntityID entity)
{
	int entToDel = -1;
	int delType = -1;
	for (unsigned int i = 0; i < PT_Unused; i++)
	{
		for (unsigned int n = 0; n < ents[i].size(); n++)
		{
			if (entityManager.entityGetComponent<ProjectileComponent>(ents[i][n]).getSource() == entity)
			{
				entityManager.entityGetComponent<ProjectileComponent>(ents[i][n]).setSource(0);
			}

			if (ents[i][n] == entity)
			{
				entToDel = n;
				delType = i;
			}
		}
	}

	if (entToDel != -1)
	{
		ents[delType].erase(ents[delType].begin() + entToDel);
	}
}

bool ProjectileSystem::BulletTick(unsigned int n, double deltaT)
{
	VelocityComponent& vel = entityManager.entityGetComponent<VelocityComponent>(ents[PT_Bullet][n]);
	Transform& trans = entityManager.entityGetComponent<TransformComponent>(ents[PT_Bullet][n]);
	BulletComponent& projectile = entityManager.entityGetComponent<BulletComponent>(ents[PT_Bullet][n]);

	glm::vec3 pos = trans.getPos();
	trans.setPos(pos + vel.getVelocity() * (float)deltaT);
	
	projectile.addTravelDist((float)deltaT * glm::length(vel.getVelocity()));
	
	if (projectile.getTravelDist() > 100.0f)
	{
		entityManager.deleteEntity(ents[PT_Bullet][n]);
		return true;
	}
	
	collisionSystem.Collide(LineSegment(pos, trans.getPos()), collisions, projectile.getEffect());
	return false;
}

bool ProjectileSystem::GroundWaveTick(unsigned int n, double deltaT)
{
	Transform& trans = entityManager.entityGetComponent<TransformComponent>(ents[PT_GroundWave][n]);
	GroundWaveComponent& projectile = entityManager.entityGetComponent<GroundWaveComponent>(ents[PT_GroundWave][n]);

	float r = projectile.getRadius();
	projectile.setRadius(r + projectile.getExpansionVelocity() * (float)deltaT);

	trans.setScl(glm::vec3(projectile.getRadius(), 0.5f, projectile.getRadius()));

	if (projectile.getMaxRadius() < projectile.getRadius())
	{
		projectile.setRadius(projectile.getMaxRadius());
	}

	collisionSystem.CollideTubeGWOpt(r, projectile.getRadius(), trans.getPos(), 0.25f, collisions, projectile.getEffect());

	if (projectile.getMaxRadius() == projectile.getRadius())
	{
		entityManager.deleteEntity(ents[PT_GroundWave][n]);
		return true;
	}

	return false;
}

void ProjectileSystem::Tick(double deltaT)
{
	for (unsigned int i = 0; i < PT_Unused; i++)
	{
		for (unsigned int n = 0; n < ents[i].size(); n++)
		{
			bool projDeleted = false;

			switch (i)
			{
			case PT_Bullet:
				projDeleted = BulletTick(n, deltaT);
				break;
			case PT_GroundWave:
				projDeleted = GroundWaveTick(n, deltaT);
				break;
			}

			if (projDeleted)
			{
				n--;
				continue;
			}

			ProjectileComponent& projectile = entityManager.entityGetComponent<ProjectileComponent>(ents[i][n]);

			for (unsigned int j = 0; j < collisions.size(); j++)
			{
				if (projectile.getEffect().Effect(collisions[j].id, entityManager, deltaT))
				{
					entityManager.deleteEntity(ents[i][n]);
					n--;
					break;
				}
			}

			collisions.clear();
		}
	}
}