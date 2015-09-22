#pragma once

#include "GameData.h"

class Render	//Performs all rendering using the GameData. Has read-only access on GameData.
{
private:
	const GameData& m_rData;
public:
	Render(const GameData& gameData) : m_rData(gameData){}
	void RenderFrame();
};