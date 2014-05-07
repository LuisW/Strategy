#pragma once

#include "ei/GL/glew.h"
#include "AssetSubManagerBase.h"

class SubShader
{
private:
	GLuint shader;
	bool isFragment;

public:
	inline SubShader(GLuint _shader, bool _isFragment) : shader(_shader), isFragment(_isFragment)
	{
	}

	inline bool isFrag()
	{
		return isFragment;
	}

	inline GLuint getShader() const
	{
		return shader;
	}
};

typedef AssetKey<SubShader> SubShaderKey;
typedef Asset<SubShader>	SubShaderAsset;
typedef const Asset<SubShader> SubShaderAsset_const;

template<>
class AssetKey<SubShader>
{
public:
	std::string name;
	bool isfrag;


	inline bool operator==(const SubShaderKey& other) const
	{
		return name == other.name && isfrag == other.isfrag;
	}

	AssetKey(const std::string& _name, bool _isfrag) : name(_name), isfrag(_isfrag)
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
			return hash<std::string>()(k.name) ^ (k.isfrag ? unsigned int(1) : unsigned int(0));
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