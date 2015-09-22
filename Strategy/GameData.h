#pragma once
#include "ei/SDL/SDL.h"
#include "ei/glm/glm.hpp"
#include "EntityManager.h"
#include "CameraComponent.h"
#include "AssetManager.h"
#include "RenderSystem.h"
#include "DataStructs.h"
#include "CullingSystem.h"
#include "PlayerControlSystem.h"
#include "CollisionSystem.h"
#include "TerrainGenerator.h"

class Update;

class GameData	//Stores all Data related to the running game. GameData and all related classes are not allowed to modify the data in any way
{
private:
	friend Update;

	bool m_keyboardState[256];
	bool m_mouseBtnState[256];

	EntityManager			m_entities;
	Camera*					m_pActiveCamera;
	EntityID				m_player1;
	RenderSystem			m_renderSystem;
	Terrain					m_terrain;
	CullingSystem			m_cullingSystem;
	CollisionSystem			m_collisionSystem;
	PlayerControlSystem		m_playerControl;
	TerrainGenerator		m_terrainGen;

	RenderSettings m_renderSettings;

	GameData(const GameData& other);
	const GameData& operator=(const GameData& other);

public:
	GameData() : m_pActiveCamera(NULL), m_renderSystem(m_entities), m_terrain(), m_cullingSystem(m_entities, TerrainSettings::LoDs, m_terrain), 
		m_playerControl(*this), m_collisionSystem(m_entities), m_terrainGen()
	{
		for (int n = 0; n < 256; n++)
		{
			m_keyboardState[n] = false;
			m_mouseBtnState[n] = false;
		}

		m_entities.RegisterSystem(&m_renderSystem);
		m_entities.RegisterSystem(&m_collisionSystem);

		m_entities.newTestObject();
		m_player1 = m_entities.newPlayer();
		m_pActiveCamera = (Camera*)(&m_entities.entityGetComponent<CameraComponent>(m_player1));

		m_terrainGen.GenerateFullTerrain();
		m_terrain.getHMapManager().Init(m_pActiveCamera->getPos());
	}

	inline bool getKeyboardState(SDLKey key) const
	{
		return m_keyboardState[key];
	}

	inline bool getMouseBtnState(Uint8 btn) const
	{
		return m_mouseBtnState[btn];
	}

	inline const EntityManager& getEntityManager() const
	{
		return m_entities;
	}

	inline EntityManager& getEntityManager()
	{
		return m_entities;
	}

	inline CollisionSystem& getCollisionSystem()
	{
		return m_collisionSystem;
	}

	inline const Camera* getActiveCam() const
	{
		return m_pActiveCamera;
	}	  

	inline const RenderSystem& getRenderSystem() const
	{
		return m_renderSystem;
	}

	inline const Terrain& getTerrain() const
	{
		return m_terrain;
	}

	inline EntityID getPlayer()
	{
		return m_player1;
	}
};