#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

class AssetManager
{
private:
	SubShaderAssetManager	subShaders;
	ShaderAssetManager		shaders;
	MeshAssetManager		meshes;
	TextureAssetManager		textures;

	static AssetManager* instance;

	static void init();
	static void deinit();
	
	friend int main(int argc, const char* argv[]);
public:
	AssetManager();

	template<typename T>
	static const Asset<T> getAsset(const AssetKey<T>& key);

	~AssetManager();
};