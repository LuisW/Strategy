#include "CreatureAISystem.h"
#include "EntityManager.h"
#include "Behaviours.h"

CreatureAISystem::CreatureAISystem(EntityManager& _entityManager, CreatureManager& _creatureManager, CollisionSystem& _collisionSystem)
: entityManager(_entityManager), creatureManager(_creatureManager), collisionSystem(_collisionSystem)
{
	entityManager.RegisterSystem(this);
}

void CreatureAISystem::onEntityChanged(EntityID entity, ComponentType type, bool added)
{
	if (type != CT_Transform && type != CT_CreatureAI && type != CT_Render && type != CT_Stats && type != CT_Vitals)
	{
		return;
	}

	if (!added)
	{
		for (unsigned int n = 0; n < entities.size(); n++)
		{
			if (entities[n] == entity)
			{
				entities.erase(entities.begin() + n);
				return;
			}
		}
	}
	else if (entityManager.entityHasComponent(entity, CT_Transform) && entityManager.entityHasComponent(entity, CT_CreatureAI)
		&& entityManager.entityHasComponent(entity, CT_Render) && entityManager.entityHasComponent(entity, CT_Stats)
		&& entityManager.entityHasComponent(entity, CT_Vitals))
	{
		entities.push_back(entity);
	}
}

void CreatureAISystem::onEntityRemoved(EntityID entity)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		if (entities[n] == entity)
		{
			entities.erase(entities.begin() + n);
		}
	}
}

#define MinViewDistFrac 0.05f
#define AverageCone 1.5708f
#define MaxConeDelta 0.5236f
#define AverageViewDist 50.0f
#define MaxDeltaViewDist 25.0f

#define CombatTimer 60000

void CreatureAISystem::Tick(Camera& player, double deltaT)
{
	BehaviourPassStruct data;
	data.setDeltaT(deltaT);
	data.setPlayer(player);

	for (unsigned int n = 0; n < entities.size(); n++)
	{
		EntityID ent = entities[n];

		CreatureAIComponent& AIcomp = entityManager.entityGetComponent<CreatureAIComponent>(ent);
		TransformComponent& trans = entityManager.entityGetComponent<TransformComponent>(ent);
		RenderComponent& rend = entityManager.entityGetComponent<RenderComponent>(ent);
		StatsComponent& stats = entityManager.entityGetComponent<StatsComponent>(ent);
		VitalsComponent& vitals = entityManager.entityGetComponent<VitalsComponent>(ent);

		glm::vec3 front = trans.getRot() * glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 distv = player.getPos() - trans.getPos();
		float dist = glm::length(distv);
		distv /= dist;

		float phi = glm::dot(front, distv);

		if (phi > 1.0f)
		{
			phi = 1.0f;
		}

		phi = glm::acos(phi);
		float cone = MaxConeDelta * stats.perception.current + AverageCone;
		float viewDist = (AverageViewDist + stats.perception.current * MaxDeltaViewDist) * (-(1.0f - MinViewDistFrac) * glm::pow(phi / cone, 4.0f) + 1.0f);

		if (phi <= cone && dist <= viewDist)
		{
			AIcomp.setMode(CAI_Combat);
			AIcomp.setCombatTimer(SDL_GetTicks());

		}
		else if (AIcomp.getMode() == CAI_Combat)
		{
			AIcomp.setMode(CAI_Search);
		}

		if (AIcomp.getMode() != CAI_Idle)
		{
			if (SDL_GetTicks() - AIcomp.getCombatTimer() >= CombatTimer)
			{
				AIcomp.setMode(CAI_Idle);
			}
		}

		data.setAIComp(AIcomp);
		data.setVitals(vitals);
		data.setEntityID(ent);
		data.setRender(rend);
		data.setTrans(trans);
		data.setStats(stats);

		glm::vec3 col;

		switch (AIcomp.getMode())
		{
		case CAI_Combat:
			AIcomp.getRangedSetup().Run(data);
			col = glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case CAI_Idle:
			AIcomp.getIdle().Run(data);
			col = glm::vec3(0.5f);
			break;
		case CAI_Search:
			col = glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		}

		rend.setColor(col);
	}
}