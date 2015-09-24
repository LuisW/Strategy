#include "VertexFormat.h"
#include "DebugHelp.h"
#include "MaterialTypeMap.h"
#include <algorithm>
#include "Mesh.h"

#pragma region FormatElems

bool ShaderFormatElem::operator<(const ShaderFormatElem& other) const
{
	if (m_semantic < other.m_semantic)
	{
		return true;
	}
	else if (m_semantic == other.m_semantic)
	{
		return m_semIndex < other.m_semIndex;
	}

	return false;
}

bool ShaderFormatElem::operator<(const VertexFormatElem& other) const
{
	if (m_semantic < other.m_semantic)
	{
		return true;
	}
	else if (m_semantic == other.m_semantic)
	{
		return m_semIndex < other.m_semIndex;
	}

	return false;
}

bool VertexFormatElem::operator<(const VertexFormatElem& other) const
{
	if (m_semantic < other.m_semantic)
	{
		return true;
	}
	else if (m_semantic == other.m_semantic)
	{
		return m_semIndex < other.m_semIndex;
	}

	return false;
}

bool VertexFormatElem::operator<(const ShaderFormatElem& other) const
{
	if (m_semantic < other.m_semantic)
	{
		return true;
	}
	else if (m_semantic == other.m_semantic)
	{
		return m_semIndex < other.m_semIndex;
	}

	return false;
}

bool VertexFormatElem::CompatibleData(const ShaderFormatElem& mat, const VertexFormatElem& mesh)
{
	return mat.m_size <= mesh.m_size && mat.m_type == mesh.m_type && mat.m_semantic == mesh.m_semantic && mat.m_semIndex == mesh.m_semIndex;
}

#pragma endregion

MeshToMaterialMap::MeshToMaterialMap(std::vector<MapElem>& mapElems, size_t vertSize, const Mesh& mesh) : m_mapElems(mapElems), m_vertSize(vertSize)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	mesh.Bind();

	for (unsigned int n = 0; n < m_mapElems.size(); n++)
	{
		glEnableVertexAttribArray(m_mapElems[n].m_loc);
		glVertexAttribPointer(m_mapElems[n].m_loc, m_mapElems[n].m_size, m_mapElems[n].m_type, GL_FALSE, m_vertSize, (void*)(m_mapElems[n].m_offset));
	}
	glBindVertexArray(0);
}

void MeshToMaterialMap::Bind() const
{
	glBindVertexArray(m_VAO);
}

VertexAttribFormat::VertexAttribFormat(size_t vertSize) : m_vertSize(vertSize == NO_PADDING ? 0 : vertSize), m_noPadd(vertSize == NO_PADDING)
{
}

const VertexFormatElem& VertexAttribFormat::operator[](size_t n) const
{
	return m_layout[n];
}

size_t VertexAttribFormat::getAttribCount() const
{
	return m_layout.size();
}

void VertexAttribFormat::addElem(const VertexFormatElem& elem)
{
	if (m_layout.size() > 0)
	{
		auto it = m_layout.begin();
		for (; it != m_layout.end() && *it < elem; it++);
		it = m_layout.insert(it, elem);
		NormalizeType(it->m_type, it->m_size);
	}
	else
	{
		m_layout.push_back(elem);
		NormalizeType(m_layout.back().m_type, m_layout.back().m_size);
	}

	if (m_noPadd)
	{
		m_vertSize += getShaderTypeSize(elem.m_type) * elem.m_size;
	}
}

void VertexAttribFormat::addElem(size_t offset, GLenum type, GLint size, Semantic semantic, unsigned int semIndex)
{
	VertexFormatElem elem;
	elem.m_offset = offset;
	elem.m_semantic = semantic;
	elem.m_semIndex = semIndex;
	elem.m_size = size;
	elem.m_type = type;
	addElem(elem);
}

size_t VertexAttribFormat::getVertSize() const
{
	return m_vertSize;
}

ShaderAttribFormat::ShaderAttribFormat(ShaderAsset_const shader)
{
	GLint count;
	GLsizei len;
	GLint size;
	GLenum type;
	GLchar name[512];
	glGetProgramiv(shader.get().getShader(), GL_ACTIVE_ATTRIBUTES, &count);
	m_layout.resize(count);

	for (int n = 0; n < count; n++)
	{
		glGetActiveAttrib(shader.get().getShader(), n, 512, &len, &size, &type, name);

		for (unsigned int i = 0; i <= SEM_USER; i++)
		{
			if (strncmp(name, SemanticNames[i], SemNameLen[i]) == 0)
			{
				m_layout[n].m_semantic = (Semantic)i;

				char* idx = name + SemNameLen[i];
				if (*idx)
				{
					sscanf_s(idx, "%i", &(m_layout[n].m_semIndex));
				}
				else
				{
					m_layout[n].m_semIndex = 0;
				}

				break;
			}

			if (SEM_USER == i)
			{
				WARNING((name + (" is no valid semantic! (" + shader.get().getVert().getKey().toString() + ")")).c_str())
				m_layout.pop_back();
				count--;
				n--;
			}
		}
		NormalizeType(type, size);

		m_layout[n].m_loc = glGetAttribLocation(shader.get().getShader(), name);
		m_layout[n].m_size = size;
		m_layout[n].m_type = type;
	}

	std::sort(m_layout.begin(), m_layout.end());
}

const ShaderFormatElem& ShaderAttribFormat::operator[](size_t n) const
{
	return m_layout[n];
}

size_t ShaderAttribFormat::getAttribCount() const
{
	return m_layout.size();
}

MeshToMaterialMap LinkMeshToMaterial(const Mesh& mesh, const ShaderAttribFormat& materialFmt)
{
	std::vector<MeshToMaterialMap::MapElem> map;
	MeshToMaterialMap::MapElem elem;

	const VertexAttribFormat& meshFmt = mesh.getFormat();

	unsigned int meIdx = 0;
	unsigned int maIdx = 0;

	while (meIdx < meshFmt.getAttribCount() && maIdx < materialFmt.getAttribCount())
	{
		if (VertexFormatElem::CompatibleData(materialFmt[maIdx], meshFmt[meIdx]))
		{
			elem.m_loc = materialFmt[maIdx].m_loc;
			elem.m_offset = meshFmt[meIdx].m_offset;
			elem.m_size = materialFmt[maIdx].m_size;
			elem.m_type = materialFmt[maIdx].m_type;

			map.push_back(elem);

			meIdx++;
			maIdx++;
		}
		else if (materialFmt[maIdx] < meshFmt[meIdx])
		{
			ERROR("Vertex format mismatch! Material requires Data the Mesh doesn't provide");
		}
		else
		{
			meIdx++;
		}
	}

	if (materialFmt.getAttribCount() > map.size())
	{
		ERROR("Vertex format mismatch! Material requires Data the Mesh doesn't provide");
	}

	return MeshToMaterialMap(map, meshFmt.getVertSize(), mesh);
}

void NormalizeType(GLenum& type, GLint& size)
{
	size *= getShaderBaseTypeSize(type);
	type = getShaderBaseType(type);
}