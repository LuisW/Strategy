#pragma once

class GameState
{
public:
	virtual bool LoopTick() = 0;
	virtual void Pop() = 0;
	virtual ~GameState(){}
};