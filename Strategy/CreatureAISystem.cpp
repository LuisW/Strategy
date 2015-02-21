#include "CreatureAISystem.h"
#include "EntityManager.h"
#include "GameData.h"

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

bool PlayerVisibleFromPt(const glm::vec3& front, const glm::vec3& pos, const glm::vec3& player, float perception)
{
	glm::vec3 distv = player - pos;
	float dist = glm::length(distv);
	distv /= dist;

	float phi = glm::dot(front, distv);

	if (phi > 1.0f)
	{
		phi = 1.0f;
	}

	phi = glm::acos(phi);
	float cone = MaxConeDelta * perception + AverageCone;
	float viewDist = (AverageViewDist + perception * MaxDeltaViewDist) * (-(1.0f - MinViewDistFrac) * glm::pow(phi / cone, 4.0f) + 1.0f);

	return (phi <= cone && dist <= viewDist);
}

void CreatureAISystem::makeAIPlan(EntityID ent)
{
	CreatureAIComponent& AIcomp = entityManager.entityGetComponent<CreatureAIComponent>(ent);
	TransformComponent& trans = entityManager.entityGetComponent<TransformComponent>(ent);
	RenderComponent& rend = entityManager.entityGetComponent<RenderComponent>(ent);
	StatsComponent& stats = entityManager.entityGetComponent<StatsComponent>(ent);
	VitalsComponent& vitals = entityManager.entityGetComponent<VitalsComponent>(ent);

	AIPlan plan = AIPlan();

	unsigned int targetSkill = 0;
	float time = INFINITY;
	float expDmg = 0;

	for (unsigned int n = 0; n < AIcomp.getAbilityCount(); n++)
	{
		IAbility& skill = AIcomp.getAbility(n);
		
		if (!(skill.getInfo().functions & AF_Damage))
		{
			continue;
		}

		float trot = glm::acos(glm::dot(skill.getCastArea().getTargetingDir(), AIcomp.getDirToPlayer())) / stats.derived.turnspeed;
		float t = (AIcomp.getDistToPlayer() - skill.getCastArea().getTargetingRange()) / stats.derived.movementspeed - 0.5f*trot;

		if (t > 0)
		{
			t += trot;
		}
		else
		{
			t = trot;
		}

		t += skill.getCastTime();
		t += skill.getCastArea().getTargetingRange() / skill.getSkillVelocity();
		t += skill.getRemainCooldown();

		if (t < time)
		{
			time = t;
			targetSkill = n;
		}
	}

	plan.push(AIPlanElement(targetSkill, false));

	AIcomp.setAIPlan(plan);
}

void CreatureAISystem::setTargetTransformation(EntityID ent, const IAbility& skill, const glm::vec3& aimPos)
{
	Transform& Tc = entityManager.entityGetComponent<TransformComponent>(ent);
	CreatureAIComponent& AIcomp = entityManager.entityGetComponent<CreatureAIComponent>(ent);

	glm::vec3 dir = glm::normalize(aimPos - Tc.getPos());

	AIcomp.setTargetPos(aimPos - dir * skill.getCastArea().getTargetingRange());
	glm::quat rot = AIFindQuatToTarget(dir, skill.getCastArea().getTargetingDir());
	AIcomp.setTargetRot(rot);
}

bool CreatureAISystem::predictHit(glm::vec3& aimPos, EntityID player, EntityID ent, const IAbility& skill)
{
	CreatureAIComponent& AIComp = entityManager.entityGetComponent<CreatureAIComponent>(ent);
	Transform& Tp = entityManager.entityGetComponent<TransformComponent>(player);
	glm::vec3 Vp = entityManager.entityGetComponent<VelocityComponent>(player).getVelocity();
	double v = skill.getSkillVelocity();

	double PCDist2 = AIComp.getDistToPlayer();
	glm::vec3 DPos = AIComp.getDirToPlayer();
	DPos *= PCDist2;
	PCDist2 *= PCDist2;

	double c = (glm::dot(Vp, Vp) - v*v);

	double t = 0.0;

	if (glm::abs(c) > M_Err)
	{
		c = 1.0 / c;
		double b = c * PCDist2;
		double a = -c * glm::dot(DPos, Vp);

		if (b > a * a)
		{
			return false;
		}

		t = a - glm::sqrt(a * a - b);

		if (t < 0.0)
		{
			t = 2.0 * a - t;
		}
	}
	else
	{
		t = glm::dot(DPos, Vp);

		if (glm::abs(t) < M_Err)
			return false;

		t = -PCDist2 / (2.0 * t);
	}

	if (t < 0.0)
	{
		return false;
	}

	aimPos = Tp.getPos() + (float)t * Vp;

	Transform& Tc = entityManager.entityGetComponent<TransformComponent>(ent);

	return skill.getCastArea().pointInside(transformToLocalSpace(aimPos, Tc));
}

int CreatureAISystem::getMoveSkill(EntityID player, EntityID ent)
{
	CreatureAIComponent& AIcomp = entityManager.entityGetComponent<CreatureAIComponent>(ent);
	
	for (int n = 0; n < AIcomp.getAbilityCount(); n++)
	{
		IAbility& skill = AIcomp.getAbility(n);
		if (skill.getInfo().functions & AF_Translation && skill.isReady())
		{
			Transform& Tc = entityManager.entityGetComponent<TransformComponent>(ent);

			if (skill.getCastArea().pointInside(transformToLocalSpace(AIcomp.getTargetPos(), Tc)))
			{
				return n;
			}
		}
	}

	return -1;
}

void CreatureAISystem::Tick(Camera& player, double deltaT, GameData& data)
{
	for (unsigned int n = 0; n < entities.size(); n++)
	{
		EntityID ent = entities[n];

		CreatureAIComponent& AIcomp = entityManager.entityGetComponent<CreatureAIComponent>(ent);
		TransformComponent& trans = entityManager.entityGetComponent<TransformComponent>(ent);
		RenderComponent& rend = entityManager.entityGetComponent<RenderComponent>(ent);
		StatsComponent& stats = entityManager.entityGetComponent<StatsComponent>(ent);
		VitalsComponent& vitals = entityManager.entityGetComponent<VitalsComponent>(ent);
		Transform& Tp = entityManager.entityGetComponent<TransformComponent>(data.getPlayer());

		glm::vec3 v = trans.getRot() * glm::vec3(-1.0f, 0.0f, 0.0f);
		AIcomp.setFront(v);
		v = Tp.getPos() - trans.getPos();
		float len = glm::length(v);
		AIcomp.setAIPlayerVec(v / len, len);

		bool vis = PlayerVisibleFromPt(v, trans.getPos(), player.getPos(), stats.perception.current);

		AIPlan& plan = AIcomp.getAIPlan();

		if (vis)
		{
			if (plan.empty())
			{
				makeAIPlan(ent);
				plan = AIcomp.getAIPlan();
			}

			IAbility& top = AIcomp.getAbility(plan.top().Ability);
			AIAbilityState state = AAS_Running;
			
			if (plan.top().running)	//execute skill if one is active
			{
				state = top._Tick(deltaT);
			}
			else  //try to activate one otherwise
			{
				glm::vec3 hitPos = Tp.getPos();
				if (predictHit(hitPos, data.getPlayer(), ent, top) && top.isReady())  //cast skill if player is in cast area
				{
					top.Cast(hitPos);
					state = top._Tick(deltaT);
					plan.top().running = true;
				}
				else												 //otherwise move to get player into cast area
				{
					setTargetTransformation(ent, top, hitPos);
					int moveablt = getMoveSkill(data.getPlayer(), ent);
					
					if (moveablt < 0)								 //if theres no move ability avaliable use the default
					{
						AIcomp.getMoveAbility().Tick(deltaT);
					}
					else
					{
						plan.push(AIPlanElement(moveablt, true));
						AIcomp.getAbility(moveablt).Cast();
						state = AIcomp.getAbility(moveablt)._Tick(deltaT);
					}
				}
			}

			if (state == AAS_Finished)
			{
				plan.pop();
			}
			else if (state == AAS_Aborted)
			{
				AIcomp.setAIPlan(AIPlan());
			}
		}
		else
		{
			
		}

		glm::vec3 col;

		col = glm::vec3(vitals.healthPercent);

		rend.setColor(col);
	}
}