#pragma once

#include "ei/GL/glew.h"
#include "AssetSubManagerBase.h"

class SubShader
{
private:
	GLuint	m_shader;
	bool	m_isFragment;

public:
	inline SubShader(GLuint shader, bool isFragment) : m_shader(shader), m_isFragment(isFragment)
	{
	}

	inline bool isFrag()
	{
		return m_isFragment;
	}

	inline GLuint getShader() const
	{
		return m_shader;
	}
};

typedef AssetKey<SubShader> SubShaderKey;
typedef Asset<SubShader>	SubShaderAsset;
typedef const Asset<SubShader> SubShaderAsset_const;

template<>
class AssetKey<SubShader>
{
public:
	std::string m_name;
	bool		m_isfrag;


	std::string toString() const
	{
		return "<SubShader>:" + m_name;
	}


	inline bool operator==(const SubShaderKey& other) const
	{
		return m_name == other.m_name && m_isfrag == other.m_isfrag;
	}

	AssetKey(const std::string& name, bool isfrag) : m_name(name), m_isfrag(isfrag)
	{

	}
};

namespace std
{
	template <>
	struct hash<SubShaderKey>
	{
		std::size_t operator()(const SubShaderKey& k) const
		{
			return hash<std::string>()(k.m_name) ^ (k.m_isfrag ? unsigned int(1) : unsigned int(0));
		}
	};

}

class SubShaderAssetManager : public AssetSubManagerBase<SubShader>
{
private:
	SubShader* Load(SubShaderKey key);
	void Unload(AssetIterator it);
public:
};