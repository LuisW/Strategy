#pragma once

#include "ei/GL/glew.h"
#include "AssetSubManagerBase.h"
#include "BoundingBox.h"
#include "VertexFormat.h"

class Mesh
{
private:
	GLuint				m_verts;
	GLuint				m_inds;
	unsigned int*		m_pIndBuff;
	char*				m_pVertBuff;
	unsigned int		m_vertlen;
	unsigned int		m_indlen;
	VertexAttribFormat	m_format;

	OBB m_BoundingVolume;


	Mesh(const Mesh&)
	{}

public:
	inline Mesh(GLuint verts, GLuint inds, unsigned int vertlen, unsigned int indlen, unsigned int* indBuff, char* vertBuff, VertexAttribFormat& format) //vertlen is float count, indlen is index count
		: m_verts(verts), m_inds(inds), m_vertlen(vertlen), m_indlen(indlen), m_pIndBuff(indBuff), m_pVertBuff(vertBuff),
		m_BoundingVolume(), m_format(format)
	{
	}

	inline Mesh(GLuint verts, GLuint inds, unsigned int vertlen, unsigned int indlen, unsigned int* indBuff, char* vertBuff, const OBB& boundingVolume, VertexAttribFormat& format) :  //vertlen is float count, indlen is index count
		m_verts(verts), m_inds(inds), m_vertlen(vertlen), m_indlen(indlen), m_pIndBuff(indBuff), m_pVertBuff(vertBuff),
		m_BoundingVolume(boundingVolume), m_format(format)
	{
	}

	inline Mesh() : m_verts(0), m_inds(0), m_pIndBuff(0), m_pVertBuff(0), m_vertlen(0), m_indlen(0)
	{}

	inline GLuint getVertices() const
	{
		return m_verts;
	}

	inline GLuint getIndices() const
	{
		return m_inds;
	}

	inline GLuint getVertlen() const
	{
		return m_vertlen;
	}

	inline GLuint getIndlen() const
	{
		return m_indlen;
	}

	inline const char* getVertBuff() const
	{
		return m_pVertBuff;
	}

	inline const unsigned int* getIndBuff() const
	{
		return m_pIndBuff;
	}

	inline const OBB& getBoundingVolume() const
	{
		return m_BoundingVolume;
	}

	inline const VertexAttribFormat& getFormat() const
	{
		return m_format;
	}

	inline void Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_verts);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_inds);
	}

	inline void clear()
	{
		delete[] m_pIndBuff;
		delete[] m_pVertBuff;

		glDeleteBuffers(1, &m_inds);
		glDeleteBuffers(1, &m_verts);
	}
};

typedef AssetKey<Mesh> MeshKey;
typedef Asset<Mesh>	MeshAsset;
typedef const Asset<Mesh> MeshAsset_const;

template<>
class AssetKey<Mesh>
{
public:
	std::string m_sName;

	std::string toString() const
	{
		return "<Mesh>:" + m_sName;
	}

	inline bool operator==(const MeshKey& other) const
	{
		return m_sName == other.m_sName;
	}

	AssetKey(const std::string& _name) : m_sName(_name)
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
			return hash<std::string>()(k.m_sName);
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