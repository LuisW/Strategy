#pragma once

#include "GameData.h"
#include "RenderSystem.h"
#include "PlayerControlSystem.h"

class Update	//Responsible for all modifications on the current GameData, doesn´t store any data iself. Has full access on the GameData.
{
private:
	GameData& data;
	PlayerControlSystem playerControl;

public:
	Update(GameData& gameData) : data(gameData), playerControl(gameData)
	{
	}
	
	bool UpdateTick();

	~Update()
	{
	}
};