#pragma once

#include "GameData.h"

class Render	//Performs all rendering using the GameData. Has read-only access on GameData.
{
private:
	const GameData& data;
public:
	Render(const GameData& gameData) : data(gameData){}
	void RenderFrame();
};