#pragma once

#include "Window.h"
#include <stack>
#include "GameState.h"
#include "InGame.h"
#include "AssetManager.h"

int main(int argc, const char* argv[])
{
	AssetManager::init();
	Window wnd(1024, 768, "Test", false);
	std::stack<GameState*> states;

	states.push(new InGame());

	bool running = true;

	unsigned int fpsint = 0;
	long t0 = SDL_GetTicks();

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

		fpsint++;

		if (fpsint == 10)
		{
			fpsint = 0;
			float dt = (float)(SDL_GetTicks() - t0) / 10.0f;
			float fps = 1000.0f / dt;
			t0 = SDL_GetTicks();
			SDL_WM_SetCaption(std::to_string(fps).c_str(), NULL);
		}
	}

	while (!states.empty())
	{
		delete states.top();
		states.pop();
	}

	AssetManager::deinit();
	return 0;
}