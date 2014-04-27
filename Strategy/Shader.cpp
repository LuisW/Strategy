#include "Shader.h"
#include "AssetManager.h"

Shader* ShaderAssetManager::Load(ShaderKey key)
{
	SubShaderAsset_const frag = AssetManager::getAsset<SubShader>(SubShaderKey(key.frag, true));
	SubShaderAsset_const vert = AssetManager::getAsset<SubShader>(SubShaderKey(key.vert, false));
	GLuint prog = glCreateProgram();
	glAttachShader(prog, frag.get().getShader());
	glAttachShader(prog, vert.get().getShader());

	return new Shader(prog, frag, vert);
}

void ShaderAssetManager::Unload(AssetIterator it)
{
	glDeleteProgram(it->second.data->getShader());
}