#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

class AssetManager
{
private:
	SubShaderAssetManager	m_subShaders;
	ShaderAssetManager		m_shaders;
	MeshAssetManager		m_meshes;
	TextureAssetManager		m_textures;
	MaterialAssetManager	m_materials;

	static AssetManager* sm_instance;

	static void init();
	static void deinit();
	
	friend int main(int argc, const char* argv[]);
public:
	AssetManager();

	template<typename T>
	static bool hasAsset(const AssetKey<T>& key);

	template<typename T>
	static const Asset<T> getAsset(const AssetKey<T>& key);

	template<typename T>
	static const Asset<T> addAsset(const AssetKey<T>& key, T* data);

	~AssetManager();
};