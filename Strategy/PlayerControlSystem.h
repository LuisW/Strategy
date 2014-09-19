#pragma once

#include "Camera.h"

class GameData;
typedef unsigned int EntityID;

class PlayerControlSystem
{
private:
	GameData& data;
	bool rUp;
	bool rmbUp;

public:
	PlayerControlSystem(GameData& _data) : data(_data), rUp(true), rmbUp(true)
	{}

	void Tick(EntityID player, double deltaT);
};