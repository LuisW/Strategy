#pragma once
#include "ei/SDL/SDL.h"
#include "ei/glm/glm.hpp"
#include "EntityManager.h"
#include "CameraComponent.h"

class Update;

class GameData	//Stores all Data related to the running game. GameData and all related classes are not allowed to modify the data in any way
{
private:
	friend Update;

	bool KeyboardState[256];
	bool MouseBtnState[256];

	EntityManager entities;
	Camera* activeCamera;
public:
	GameData() : activeCamera(NULL)
	{
		for (int n = 0; n < 256; n++)
		{
			KeyboardState[n] = false;
			MouseBtnState[n] = false;
		}

		Entity& player1 = entities.newPlayer();
		activeCamera = (CameraComponent*)(&player1.getComponent(CT_Camera));
	}

	inline bool getKeyboardState(SDLKey key) const
	{
		return KeyboardState[key];
	}

	inline bool getMouseBtnState(Uint8 btn) const
	{
		return MouseBtnState[btn];
	}

	inline const EntityManager& getEntityManager()
	{
		return entities;
	}

	inline const Camera* getActiveCam() const
	{
		return activeCamera;
	}
};