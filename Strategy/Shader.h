#pragma once

#include "SubShader.h"
#include <string>

class Shader
{
private:
	GLuint shader;
	const Asset<SubShader> fragment;
	const Asset<SubShader> vertex;

public:
	Shader(GLuint _shader, SubShaderAsset_const frag, SubShaderAsset_const vert) : shader(_shader), fragment(frag), vertex(vert)
	{

	}

	inline GLuint getShader() const
	{
		return shader;
	}

	inline const Asset<SubShader>& getFrag()
	{
		return fragment;
	}

	inline const Asset<SubShader>& getVert()
	{
		return vertex;
	}
};

typedef AssetKey<Shader> ShaderKey;
typedef Asset<Shader> ShaderAsset;
typedef Asset<Shader> ShaderAsset_const;

template<>
class AssetKey<Shader>
{
public:
	std::string frag;
	std::string vert;

	AssetKey(const std::string& _frag, const std::string& _vert) : frag(_frag), vert(_vert)
	{

	}

	bool operator==(const AssetKey<Shader>& other) const
	{
		return frag == other.frag && vert == other.vert;
	}
};

namespace std
{

	template <>
	struct hash<ShaderKey>
	{
		std::size_t operator()(const ShaderKey& k) const
		{
			return hash<std::string>()(k.vert + k.frag);
		}
	};

}

class ShaderAssetManager : public AssetSubManagerBase<Shader>
{
private:
	Shader* Load(ShaderKey key);
	void Unload(AssetIterator it);
public:
};