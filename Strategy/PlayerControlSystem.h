#pragma once

#include "Camera.h"
#include "GameData.h"

class PlayerControlSystem
{
private:
	GameData& data;
	TextureAsset_const hmap;

	float sampleHeight(glm::vec3 pos);

public:
	PlayerControlSystem(GameData& _data) : data(_data), hmap(AssetManager::getAsset<Texture>(TextureKey("heightmap.png")))
	{}

	void Tick(EntityID player, double deltaT);
};