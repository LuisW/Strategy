#pragma once

#include "Component.h"
#include "ei/glm/glm.hpp"
#include "CreatureManager.h"
#include "AIHelp.h"

class CreatureAIComponent : public Component
{
private:
	CreatureID creatureType;

	glm::vec3 targetPos;
	glm::quat targetRot;
	glm::vec3 dirToPlayer;
	float distToPlayer;
	glm::vec3 frontVec;
	
	std::vector<IAbility*> abilities;
	IAbility* moveAbility;
	AIPlan plan;

public:
	CreatureAIComponent(EntityID owner, CreatureID _creatureType, CreatureManager& creatures, glm::vec3& location);

	inline const glm::vec3& getDirToPlayer()
	{
		return dirToPlayer;
	}

	inline float getDistToPlayer()
	{
		return distToPlayer;
	}

	inline void setAIPlayerVec(glm::vec3 dir, float dist)
	{
		distToPlayer = dist;
		dirToPlayer = dir;
	}

	inline const glm::vec3& getFront()
	{
		return frontVec;
	}
	
	inline void setFront(const glm::vec3& front)
	{
		frontVec = front;
	}

	inline const glm::vec3& getTargetPos()
	{
		return targetPos;
	}

	inline void setTargetPos(const glm::vec3 pos)
	{
		targetPos = pos;
	}

	inline const glm::quat& getTargetRot()
	{
		return targetRot;
	}

	inline void setTargetRot(const glm::quat& rot)
	{
		targetRot = rot;
	}

	inline unsigned int getAbilityCount()
	{
		return abilities.size();
	}

	inline IAbility& getAbility(unsigned int n)
	{
		return *(abilities[n]);
	}

	inline IAbility& getMoveAbility()
	{
		return *(moveAbility);
	}

	inline AIPlan& getAIPlan()
	{
		return plan;
	}

	inline void setAIPlan(const AIPlan& _plan)
	{
		plan = AIPlan(_plan);
	}

	inline CreatureID getCreatureType()
	{
		return creatureType;
	}

	static ComponentType getComponentType()
	{
		return CT_CreatureAI;
	}

	~CreatureAIComponent();
};