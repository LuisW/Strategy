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
protected:
	unsigned int endTime;
	bool infinite;

public:
	IStatEffect(unsigned int _endTime, bool _infinite) : endTime(_endTime), infinite(_infinite)
	{}

	bool hasEnded();
	virtual bool isApplicable();
	virtual bool statEffect(StatsStruct& stats, EntityID ent, double deltaT) = 0;
	virtual bool derivedEffect(StatsStruct& stats, EntityID ent, double deltaT) = 0;
};
