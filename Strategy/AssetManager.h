#pragma once

#include "Shader.h"

class AssetManager
{
private:
	SubShaderAssetManager	subShaders;
	ShaderAssetManager		shaders;

	static AssetManager* reference;

	static void init();
	static void deinit();
public:

	AssetManager();

	template<typename T>
	static const Asset<T>& getAsset(AssetKey<T> key);

	~AssetManager();
};