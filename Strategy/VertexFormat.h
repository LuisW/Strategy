#pragma once

#include "Shader.h"

class Mesh;

enum Semantic { SEM_POSITION, SEM_NORMAL, SEM_TEXCOORD, SEM_TANGENT, SEM_COLOR, SEM_WEIGHT, SEM_USER };

class MeshToMaterialMap
{
public:
	struct MapElem
	{
		size_t m_offset;
		GLint m_loc;
		GLenum m_type;
		GLint m_size;
	};

private:
	std::vector<MapElem> m_mapElems;
	size_t m_vertSize;
	GLuint m_VAO;

public:
	MeshToMaterialMap(std::vector<MapElem>& mapElems, size_t vertSize, const Mesh& mesh);
	void Bind() const;
};

struct VertexFormatElem;

struct ShaderFormatElem
{
	GLint m_loc;
	GLenum m_type;
	GLint m_size;
	Semantic m_semantic;
	unsigned int m_semIndex;

	bool operator<(const ShaderFormatElem& other) const;
	bool operator<(const VertexFormatElem& other) const;
};


struct VertexFormatElem
{
	size_t m_offset;
	GLenum m_type;
	GLint m_size;
	Semantic m_semantic;
	unsigned int m_semIndex;

	bool operator<(const VertexFormatElem& other) const;
	bool operator<(const ShaderFormatElem& other) const;
	static bool CompatibleData(const ShaderFormatElem& mat, const VertexFormatElem& mesh);
};

class VertexAttribFormat
{
private:
	std::vector<VertexFormatElem> m_layout;
	size_t m_vertSize;
	bool m_noPadd;

public:
	static const size_t NO_PADDING = SIZE_MAX;

	VertexAttribFormat(size_t vertSize = NO_PADDING);
	const VertexFormatElem& operator[](size_t n) const;
	size_t getAttribCount() const;
	void addElem(const VertexFormatElem& elem);
	void addElem(size_t offset, GLenum type, GLint size, Semantic semantic, unsigned int semIndex);
	size_t getVertSize() const;
};

class ShaderAttribFormat
{
private:
	std::vector<ShaderFormatElem> m_layout;

public:
	ShaderAttribFormat(ShaderAsset_const shader);
	const ShaderFormatElem& operator[](size_t n) const;
	size_t getAttribCount() const;
};

MeshToMaterialMap LinkMeshToMaterial(const Mesh& mesh, const ShaderAttribFormat& materialFmt);
void NormalizeType(GLenum& type, GLint& size);