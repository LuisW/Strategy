#include "ProjectileSystem.h"
#include "ProjectileComponent.h"

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
		if (entityManager.entityHasComponent(entity, CT_Projectile) && entityManager.entityHasComponent(entity, CT_Velocity))
		{
			ents.push_back(entity);
		}
	}
	else
	{
		for (unsigned int n = 0; n < ents.size(); n++)
		{
			if (ents[n] == entity)
			{
				ents.erase(ents.begin() + n);
			}
		}
	}
}

void ProjectileSystem::onEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < ents.size(); n++)
	{
		if (ents[n] == entity)
		{
			ents.erase(ents.begin() + n);
		}
	}
}

void ProjectileSystem::Tick(double deltaT)
{
	for (unsigned int n = 0; n < ents.size(); n++)
	{
		ProjectileComponent& projectile = entityManager.entityGetComponent<ProjectileComponent>(ents[n]);
		VelocityComponent& vel = entityManager.entityGetComponent<VelocityComponent>(ents[n]);

		glm::vec3 pos = projectile.getPos();
		projectile.setPrevPos(pos);
		projectile.setPos(pos + vel.getVelocity() * (float)deltaT);
		
		std::vector<CollDistData> collisions;

		collisionSystem.Collide(LineSegment(pos, projectile.getPrevPos()), collisions, projectile.getEffect());

		for (unsigned int n = 0; n < collisions.size(); n++)
		{
			if (projectile.getEffect().Effect(collisions[n].id))
			{
				entityManager.deleteEntity(ents[n]);
				continue;
			}
		}
	}
}