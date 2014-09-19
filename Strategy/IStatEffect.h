#pragma once

#include "ei\SDL\SDL.h"

typedef unsigned int EntityID;

struct Stat
{
	float standart;
	float current;
};

struct DerivedStatsStruct
{
	float turnspeed;
	float movementspeed;
};

struct StatsStruct
{
	Stat perception;
	Stat agility;
	Stat intelligence;
	Stat stamina;
	Stat vitality;
	Stat strength;
	Stat power;

	DerivedStatsStruct derived;
};

class IStatEffect
{
private:
	unsigned int endTime;
	bool infinite;

public:
	bool hasEnded();
	virtual bool isApplicable();
	virtual bool effect(EntityID entity, double deltaT) = 0;
};
