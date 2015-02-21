#include "IAbility.h"
#include "GameData.h"
#include "ProjectileComponent.h"

GameData* IAbility::data = NULL;

void IAbility::setGameData(GameData* _data)
{
	data = _data;
}

#define getComp(cmp,ent) data->getEntityManager().entityGetComponent<cmp>(ent)

EntityID IAbility::getCaster()
{
	return ent;
}

StatsComponent& IAbility::getStats(EntityID entity)
{
	return getComp(StatsComponent, entity);
}

TransformComponent& IAbility::getTransform(EntityID entity)
{
	return getComp(TransformComponent, entity);
}

VelocityComponent& IAbility::getVelocity(EntityID entity)
{
	return getComp(VelocityComponent, entity);
}

CollisionComponent& IAbility::getCollision(EntityID entity)
{
	return getComp(CollisionComponent, entity);
}

EntityID IAbility::getPlayer()
{
	return data->getPlayer();
}

float IAbility::getDistToPlayer()
{
	return getComp(CreatureAIComponent, ent).getDistToPlayer();
}

const glm::vec3& IAbility::getDirectionToPlayer()
{
	return getComp(CreatureAIComponent, ent).getDirToPlayer();
}

const glm::vec3& IAbility::getWorldSpaceFrontVec()
{
	return getComp(CreatureAIComponent, ent).getFront();
}

void IAbility::setCastArea(const AIArea& area)
{
	castArea = area;
}

void IAbility::spawnBullet(const glm::vec3& pos, const glm::vec3& vel, IProjectileEffect* effect)
{
	data->getEntityManager().newBullet(ent, pos, vel, effect);
}

void IAbility::spawnShockWave(const glm::vec3& pos, float vel, IProjectileEffect* effect)
{
	data->getEntityManager().newGroundWave(ent, pos, vel, 50.0f, effect);
}

const glm::vec3& IAbility::getMoveTarget()
{
	return getComp(CreatureAIComponent, ent).getTargetPos();
}

const glm::quat& IAbility::getRotTarget()
{
	return getComp(CreatureAIComponent, ent).getTargetRot();
}