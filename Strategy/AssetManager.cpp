#include "AssetManager.h"

AssetManager* AssetManager::instance;

AssetManager::AssetManager()
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

template<>
const Asset<Mesh> AssetManager::getAsset<Mesh>(const AssetKey<Mesh>& key)
{
	return instance->meshes.GetAsset(key);
}

template<>
const Asset<Texture> AssetManager::getAsset<Texture>(const AssetKey<Texture>& key)
{
	return instance->textures.GetAsset(key);
}

void AssetManager::deinit()
{
	instance->meshes.DeleteAll();
	instance->shaders.DeleteAll();
	instance->subShaders.DeleteAll();
	instance->textures.DeleteAll();
	delete instance;
}

AssetManager::~AssetManager()
{

}