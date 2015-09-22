#include "Shader.h"
#include "AssetManager.h"

Shader* ShaderAssetManager::Load(ShaderKey key)
{
	SubShaderAsset_const frag = AssetManager::getAsset(SubShaderKey(key.m_frag, true));
	SubShaderAsset_const vert = AssetManager::getAsset(SubShaderKey(key.m_vert, false));
	GLuint prog = glCreateProgram();
	glAttachShader(prog, frag.get().getShader());
	glAttachShader(prog, vert.get().getShader());
	glLinkProgram(prog);

	return new Shader(prog, frag, vert);
}

void ShaderAssetManager::Unload(AssetIterator it)
{
	glDeleteProgram(it->second.m_pData->getShader());
}