#pragma once

#include <functional>
#include <vector>
#include "ei/glm/glm.hpp"

enum BehaviourType {BT_Idle, BT_RangedSetup, BT_Unused};
enum BehaviourFunction { BF_NonCombat = 0, BF_Damage = 1, BF_Stun = 2, BF_Slow = 4, BF_Heal = 8, BF_Move = 16 };

class BehaviourID
{
private:
	unsigned int type;
	unsigned int index;

public:
	BehaviourID() : type(0), index(0)
	{}

	BehaviourID(unsigned int _type, unsigned int _index) : type(_type), index(_index)
	{}

	inline BehaviourType getType()
	{
		return (BehaviourType)(type);
	}

	inline unsigned int getIndex()
	{
		return index;
	}
};

class IBehaviour;
class IAbility;

typedef std::function<IBehaviour*(glm::vec3&)> NewBahviourFunc;

struct BehaviourInfo
{
	std::function<IBehaviour*(glm::vec3&)> mkNew;
	BehaviourType type;
	bool ranged;
	BehaviourFunction funcs;

	BehaviourInfo(NewBahviourFunc _mkNew, BehaviourType _type, bool _ranged, BehaviourFunction _funcs) 
		: mkNew(_mkNew), type(_type), ranged(_ranged), funcs(_funcs)
	{
	}
};

template<typename T> IBehaviour* mkBehavInst(glm::vec3& location)
{
	return (IBehaviour*)(new T(location));
}

class BehaviourManager
{
private:
	std::vector<std::vector<BehaviourInfo>> behaviours;
	
public:
	BehaviourManager();

	template<typename T>
	void addBehaviour(BehaviourType _type, bool _ranged = true, BehaviourFunction _function = BF_NonCombat)
	{
		behaviours[_type].push_back(BehaviourInfo(mkBehavInst<T>, _type, _ranged, _function));
	}

	IBehaviour* getNewInstanceOf(BehaviourID ID, glm::vec3& location);

	bool getRanged(BehaviourID ID)
	{
		behaviours[ID.getType()][ID.getIndex()].ranged;
	}

	BehaviourFunction getFunction(BehaviourID ID)
	{
		behaviours[ID.getType()][ID.getIndex()].funcs;
	}

	unsigned int getBehaviourCount(BehaviourType type)
	{
		return behaviours[type].size();
	}
};