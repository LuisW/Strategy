#pragma once

#include "Camera.h"

class GameData;
typedef unsigned int EntityID;

class PlayerControlSystem
{
private:
	GameData&		m_rData;
	bool			m_rUp;
	bool			m_tUp;
	unsigned int	m_fullAuto;

public:
	PlayerControlSystem(GameData& data) : m_rData(data), m_rUp(true), m_tUp(true), m_fullAuto(0)
	{}

	void Tick(EntityID player, double deltaT);
};