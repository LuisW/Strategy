#pragma once

#include "Camera.h"
#include "GameData.h"

class PlayerControlSystem
{
private:
	GameData& data;
public:
	PlayerControlSystem(GameData& _data) : data(_data)
	{}

	void Tick(Camera& activeCam);
};