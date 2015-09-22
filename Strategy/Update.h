#pragma once

#include "GameData.h"
#include "RenderSystem.h"

class Update	//Responsible for all modifications on the current GameData, doesn´t store any data iself. Has full access on the GameData.
{
private:
	GameData& m_rData;

public:
	Update(GameData& gameData) : m_rData(gameData)
	{
	}
	
	bool UpdateTick(double deltaT);

	~Update()
	{
	}
};