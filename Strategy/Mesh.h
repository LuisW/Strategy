#pragma once

#include "ei/GL/glew.h"
#include "AssetSubManagerBase.h"

class Mesh
{
private:
	GLuint verts;
	GLuint inds;
	unsigned int* indBuff;
	char* vertBuff;
	unsigned int vertlen;
	unsigned int indlen;

	Mesh(const Mesh&)
	{}

public:
	inline Mesh(GLuint _verts, GLuint _inds, unsigned int _vertlen, unsigned int _indlen, unsigned int* _indBuff, char* _vertBuff) : verts(_verts), inds(_inds), vertlen(_vertlen), indlen(_indlen), indBuff(_indBuff), vertBuff(_vertBuff)
	{
	}

	inline GLuint getVertices() const
	{
		return verts;
	}

	inline GLuint getIndices() const
	{
		return inds;
	}

	inline GLuint getVertlen() const
	{
		return vertlen;
	}

	inline GLuint getIndlen() const
	{
		return indlen;
	}

	inline const char* getVertBuff() const
	{
		return vertBuff;
	}

	inline const unsigned int* getIndBuff() const
	{
		return indBuff;
	}
};

typedef AssetKey<Mesh> MeshKey;
typedef Asset<Mesh>	MeshAsset;
typedef const Asset<Mesh> MeshAsset_const;

template<>
class AssetKey<Mesh>
{
public:
	std::string name;

	inline bool operator==(const MeshKey& other) const
	{
		return name == other.name;
	}

	AssetKey(const std::string& _name) : name(_name)
	{

	}
};

namespace std
{

	template <>
	struct hash<MeshKey>
	{
		std::size_t operator()(const MeshKey& k) const
		{
			return hash<std::string>()(k.name);
		}
	};

}

class MeshAssetManager : public AssetSubManagerBase<Mesh>
{
private:
	Mesh* Load(MeshKey key);
	void Unload(AssetIterator it);
public:
};