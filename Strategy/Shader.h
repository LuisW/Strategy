#pragma once

#include "SubShader.h"
#include <string>

class Shader
{
private:
	GLuint					m_shader;
	const Asset<SubShader>	m_fragment;
	const Asset<SubShader>	m_vertex;

public:
	Shader(GLuint shader, SubShaderAsset_const frag, SubShaderAsset_const vert) : m_shader(shader), m_fragment(frag), m_vertex(vert)
	{

	}

	inline GLuint getShader() const
	{
		return m_shader;
	}

	inline const Asset<SubShader>& getFrag() const
	{
		return m_fragment;
	}

	inline const Asset<SubShader>& getVert() const
	{
		return m_vertex;
	}
};

typedef AssetKey<Shader> ShaderKey;
typedef Asset<Shader> ShaderAsset;
typedef const Asset<Shader> ShaderAsset_const;

template<>
class AssetKey<Shader>
{
public:
	std::string m_frag;
	std::string m_vert;


	std::string toString() const
	{
		return "<Shader>:" + m_frag + ", " + m_vert;
	}

	AssetKey(const std::string& frag, const std::string& vert) : m_frag(frag), m_vert(vert)
	{

	}

	bool operator==(const AssetKey<Shader>& other) const
	{
		return m_frag == other.m_frag && m_vert == other.m_vert;
	}
};

namespace std
{

	template <>
	struct hash<ShaderKey>
	{
		std::size_t operator()(const ShaderKey& k) const
		{
			return hash<std::string>()(k.m_vert + k.m_frag);
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