#pragma once

#include "GameData.h"

class Update	//Responsible for all modifications on the current GameData, doesn´t store any data iself. Has full access on the GameData.
{
private:
	GameData& data;
public:
	Update(GameData& gameData) : data(gameData){}
	bool UpdateTick();
};