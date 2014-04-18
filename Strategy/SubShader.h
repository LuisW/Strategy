#pragma once

#include "ei/GL/glew.h"
#include "AssetSubManagerBase.h"

class SubShader
{
private:
	GLuint shader;
	bool isFragment;

public:
	inline bool isFrag()
	{
		return isFragment;
	}

	inline GLuint getShader()
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
};

namespace std {

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