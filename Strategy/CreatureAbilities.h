#pragma once

#include <vector>
#include <functional>
#include "IAbility.h"

typedef unsigned int AbilityID;

class CreatureAbilityManager
{
private:
	template<typename T>
	static IAbility* newAbilityInst(AbilityInfo& info, EntityID ent)
	{
		IAbility* ptr = (IAbility*)(new T(ent));
		ptr->setInfo(info);
		return ptr;
	}

	struct AbilityListElem
	{
		AbilityInfo info;
		std::function<IAbility*(AbilityInfo&, EntityID)> mknew;
	};

	std::vector<AbilityListElem> abilities;
	std::vector<AbilityListElem> baseMoveAbilities;

public:
	CreatureAbilityManager(GameData* data);

	template<typename T> 
	void AddAbility(unsigned int functions)
	{
		abilities.push_back(AbilityListElem());
		abilities.back().info.functions = functions;
		abilities.back().mknew = newAbilityInst<T>;
	}

	template<typename T>
	void AddBaseMoveAbility()
	{
		baseMoveAbilities.push_back(AbilityListElem());
		baseMoveAbilities.back().info.functions = AF_Translation;
		baseMoveAbilities.back().mknew = newAbilityInst<T>;
	}

	IAbility* makeNewInstance(AbilityID ability, EntityID entity)
	{
		return abilities[ability].mknew(abilities[ability].info, entity);
	}

	IAbility* makeNewBaseMoveInstance(AbilityID ability, EntityID entity)
	{
		return baseMoveAbilities[ability].mknew(abilities[ability].info, entity);
	}

	unsigned int getAbilityCount()
	{
		return abilities.size();
	}

	const AbilityInfo& getInfo(AbilityID ability)
	{
		return abilities[ability].info;
	}
};