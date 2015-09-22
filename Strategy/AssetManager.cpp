#include "AssetManager.h"

AssetManager* AssetManager::sm_instance;

AssetManager::AssetManager()
{

}

void AssetManager::init()
{
	sm_instance = new AssetManager();
}

#pragma region hasAsset
template<>
bool AssetManager::hasAsset<Mesh>(const AssetKey<Mesh>& key)
{
	return sm_instance->m_meshes.hasAsset(key);
}

template<>
bool AssetManager::hasAsset<Texture>(const AssetKey<Texture>& key)
{
	return sm_instance->m_textures.hasAsset(key);
}

#pragma endregion

#pragma region getAsset
template<>
const Asset<Shader> AssetManager::getAsset<Shader>(const AssetKey<Shader>& key)
{
	return sm_instance->m_shaders.GetAsset(key);
}

template<>
const Asset<SubShader> AssetManager::getAsset<SubShader>(const AssetKey<SubShader>& key)
{
	return sm_instance->m_subShaders.GetAsset(key);
}

template<>
const Asset<Mesh> AssetManager::getAsset<Mesh>(const AssetKey<Mesh>& key)
{
	return sm_instance->m_meshes.GetAsset(key);
}

template<>
const Asset<Texture> AssetManager::getAsset<Texture>(const AssetKey<Texture>& key)
{
	return sm_instance->m_textures.GetAsset(key);
}

template<>
const Asset<Material> AssetManager::getAsset<Material>(const AssetKey<Material>& key)
{
	return sm_instance->m_materials.GetAsset(key);
}
#pragma endregion

#pragma region addAsset
template<>
const Asset<Mesh> AssetManager::addAsset<Mesh>(const AssetKey<Mesh>& key, Mesh* data)
{
	return sm_instance->m_meshes.AddAsset(key, data);
}

template<>
const Asset<Texture> AssetManager::addAsset<Texture>(const AssetKey<Texture>& key, Texture* data)
{
	return sm_instance->m_textures.AddAsset(key, data);
}
#pragma endregion

void AssetManager::deinit()
{
	sm_instance->m_materials.DeleteAll();
	sm_instance->m_meshes.DeleteAll();
	sm_instance->m_shaders.DeleteAll();
	sm_instance->m_subShaders.DeleteAll();
	sm_instance->m_textures.DeleteAll();
	delete sm_instance;
}

AssetManager::~AssetManager()
{

}