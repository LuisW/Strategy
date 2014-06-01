#pragma once
#include "ei/SDL/SDL.h"
#include "ei/glm/glm.hpp"
#include "EntityManager.h"
#include "CameraComponent.h"
#include "AssetManager.h"
#include "RenderSystem.h"
#include "DataStructs.h"
#include "VisibilitySystem.h"

class Update;

class GameData	//Stores all Data related to the running game. GameData and all related classes are not allowed to modify the data in any way
{
private:
	friend Update;

	bool KeyboardState[256];
	bool MouseBtnState[256];

	EntityManager entities;
	Camera* activeCamera;
	EntityID player1;
	ShaderAsset_const shader;
	RenderSystem renderSystem;
	Terrain terrain;
	VisibilitySystem visibilitySystem;

	RenderSettings renderSettings;

public:
	GameData() : activeCamera(NULL), shader(AssetManager::getAsset<Shader>(ShaderKey("test.frag", "test.vert"))), renderSystem(entities), terrain()
		, visibilitySystem(entities, TerrainSettings::LoDs, terrain)
	{
		for (int n = 0; n < 256; n++)
		{
			KeyboardState[n] = false;
			MouseBtnState[n] = false;
		}

		entities.RegisterSystem(&renderSystem);

		entities.newTestObject();
		player1 = entities.newPlayer();
		activeCamera = (Camera*)(&entities.entityGetComponent<CameraComponent>(player1));  
	}

	inline bool getKeyboardState(SDLKey key) const
	{
		return KeyboardState[key];
	}

	inline bool getMouseBtnState(Uint8 btn) const
	{
		return MouseBtnState[btn];
	}

	inline const EntityManager& getEntityManager() const
	{
		return entities;
	}

	inline EntityManager& getEntityManager()
	{
		return entities;
	}

	inline const Camera* getActiveCam() const
	{
		return activeCamera;
	}	  

	inline ShaderAsset_const getShader() const
	{
		return shader;
	}

	inline const RenderSystem& getRenderSystem() const
	{
		return renderSystem;
	}

	inline const Terrain& getTerrain() const
	{
		return terrain;
	}

	inline EntityID getPlayer()
	{
		return player1;
	}
};