#pragma once

#include "GameState.h"
#include "GameData.h"
#include "Render.h"
#include "Update.h"
#include "Timer.h"

class InGame : public GameState
{
private:
	GameData	m_data;
	Render		m_render;
	Update		m_update;

	Timer m_timer;

public:
	InGame() : m_data(), m_render(m_data), m_update(m_data)
	{
	}

	inline bool LoopTick()
	{
		double deltaT = m_timer.Stop();
		m_timer.Start();
		bool res = m_update.UpdateTick(deltaT);

		if (res)
		{
			m_render.RenderFrame();
		}

		return res;
	}

	inline void Pop()
	{

	}

	~InGame()
	{
	}
};