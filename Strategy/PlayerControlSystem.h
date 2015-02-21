#pragma once

#include "Camera.h"

class GameData;
typedef unsigned int EntityID;

class PlayerControlSystem
{
private:
	GameData& data;
	bool rUp;
	bool tUp;
	unsigned int fullAuto;

public:
	PlayerControlSystem(GameData& _data) : data(_data), rUp(true), tUp(true), fullAuto(0)
	{}

	void Tick(EntityID player, double deltaT);
};