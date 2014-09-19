#pragma once

#include "Component.h"
#include "IStatEffect.h"
#include <vector>

typedef unsigned int StatEffectID;

class StatsComponent : public Component, public StatsStruct
{
private:
	std::vector<IStatEffect*> statEffects;
	
	unsigned int it;

public:
	StatsComponent(EntityID owner, const StatsStruct& stats) : Component(CT_Stats, owner, ""), StatsStruct(stats)
	{
	}

	void ResetIterator()
	{
		it = 0;
		while (it < statEffects.size() && statEffects[it] == NULL)
		{
			it++;
		}
	}

	bool ValidIterator()
	{
		return it < statEffects.size();
	}

	unsigned int getNext()
	{
		unsigned int ret = it;

		do
		{
			it++;
		} 
		while (it < statEffects.size() && statEffects[it] == NULL);

		return ret;
	}

	IStatEffect& getEffect(StatEffectID effect)
	{
		return *(statEffects[effect]);
	}

	StatEffectID AddEffect(IStatEffect* effect)
	{
		for (unsigned int n = 0; n < statEffects.size(); n++)
		{
			if (statEffects[n] == NULL)
			{
				statEffects[n] = effect;
				return n;
			}
		}

		statEffects.push_back(effect);
	}

	void RemoveEffect(StatEffectID effect)
	{
		delete statEffects[effect];
		statEffects[effect] = NULL;
	}

	void RemoveAllEffects()
	{
		for (unsigned int n = 0; n < statEffects.size(); n++)
		{
			delete statEffects[n];
		}
	}

	static ComponentType getComponentType()
	{
		return CT_Stats;
	}

	~StatsComponent()
	{
		for (unsigned int n = 0; n < statEffects.size(); n++)
		{
			delete statEffects[n];
		}
	}
};