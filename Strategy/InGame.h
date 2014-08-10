#pragma once

#include "GameState.h"
#include "GameData.h"
#include "Render.h"
#include "Update.h"
#include "Timer.h"

class InGame : public GameState
{
private:
	GameData data;
	Render render;
	Update update;

	Timer timer;

public:
	InGame() : data(), render(data), update(data)
	{
	}

	inline bool LoopTick()
	{
		double deltaT = timer.Stop();
		timer.Start();
		bool res = update.UpdateTick(deltaT);

		if (res)
		{
			render.RenderFrame();
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