#include "AssetManager.h"

AssetManager::AssetManager() : shaders(), subShaders()
{

}


void AssetManager::init()
{
	instance = new AssetManager();
}

template<>
const Asset<Shader> AssetManager::getAsset<Shader>(const AssetKey<Shader>& key)
{
	return instance->shaders.GetAsset(key);
}

template<>
const Asset<SubShader> AssetManager::getAsset<SubShader>(const AssetKey<SubShader>& key)
{
	return instance->subShaders.GetAsset(key);
}

void AssetManager::deinit()
{
	delete instance;
}

AssetManager::~AssetManager()
{

}