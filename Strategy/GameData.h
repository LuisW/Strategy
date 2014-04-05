#pragma once
#include "ei/SDL/SDL.h"

class Update;

class GameData	//Stores all Data related to the running game. GameData and all related classes are not allowed to modify the data in any way
{
private:
	friend Update;

	bool KeyboardState[256];
	bool MouseBtnState[256];
public:
	GameData()
	{
		for (int n = 0; n < 256; n++)
		{
			KeyboardState[n] = false;
			MouseBtnState[n] = false;
		}
	}

	inline bool getKeyboardState(SDLKey key) const
	{
		return KeyboardState[key];
	}

	inline bool getMouseBtnState(Uint8 btn) const
	{
		return MouseBtnState[btn];
	}
};