#pragma once

#include "Shader.h"

class AssetManager
{
private:
	SubShaderAssetManager	subShaders;
	ShaderAssetManager		shaders;

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