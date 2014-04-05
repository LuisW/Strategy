#pragma once

#include "GameState.h"
#include "GameData.h"
#include "Render.h"
#include "Update.h"

class InGame : public GameState
{
private:
	GameData data;
	Render render;
	Update update;

public:
	InGame() : data(), render(data), update(data)
	{

	}

	inline bool LoopTick()
	{
		bool res = update.UpdateTick();

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