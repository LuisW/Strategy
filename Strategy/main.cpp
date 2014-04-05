#pragma once

#include "Window.h"
#include <stack>
#include "GameState.h"
#include "InGame.h"

int main(int argc, const char* argv[])
{
	Window wnd(1024, 768, "Test", false);
	std::stack<GameState*> states;

	states.push(new InGame());

	bool running = true;

	while (running)
	{
		if(!states.top()->LoopTick())
		{
			states.top()->Pop();
			delete states.top();
			states.pop();
		}

		if (states.empty())
		{
			running = false;
		}
	}

	return 0;
}