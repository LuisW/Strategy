#pragma once

#include "MaterialTypeMap.h"
#include "VertexFormat.h"

typedef unsigned int MaterialID;
typedef int UniformID;

class Material;

struct ObjUniform
{
	GLenum type;
	size_t size;
	int uniform;
	std::string name;
	GLbyte* value;

	GLuint texIdx;
};

class MaterialObjectData
{
private:
	friend Material;

	MaterialID m_parent;
	std::vector<ObjUniform> m_uniforms;
	bool checkTypeMatching(UniformID uniform, GLenum type, GLint size) const;

public:
	MaterialObjectData(const std::vector<ObjUniform>& uniforms, MaterialID parent);
	MaterialObjectData(const MaterialObjectData& other);
	MaterialObjectData(MaterialObjectData&& other);
	const MaterialObjectData& operator=(const MaterialObjectData& rhs);

	UniformID checkUniform(const std::string& uniform) const;

	template<typename T, size_t N>
	void setUniform(UniformID uniform, const T(&value)[N]) const
	{
		if (uniform < 0)
		{
			return;
		}

		if (uniform >= m_uniforms.size())
		{
			ERROR("Attempt to set uniform with invalid UniformID")
				return;
		}

		if (checkTypeMatching(uniform, getShaderType<T>(), N))
		{
			memcpy(m_uniforms[uniform].value, value, sizeof(T)* N);
		}
		else
		{
			ERROR("Attempt to set uniform with incompatible Data")
				return;
		}
	}

	template<typename T>
	void setUniform(UniformID uniform, const T& value) const
	{
		if (uniform < 0)
		{
			return;
		}

		if (uniform >= m_uniforms.size())
		{
			ERROR("Attempt to set uniform with invalid UniformID")
				return;
		}

		if (checkTypeMatching(uniform, getShaderType<T>(), 1))
		{
			memcpy(m_uniforms[uniform].value, &value, sizeof(T));
		}
		else
		{
			ERROR("Attempt to set uniform with incompatible Data")
				return;
		}
	}

	~MaterialObjectData();
};


class Material
{
public:
	struct MaterialUniformInfo
	{
		GLenum type;
		size_t size;
		std::string name;
		const GLbyte* value;
	};

private:
	struct MatUniform
	{
		GLenum type;
		size_t size;
		int uniform;
		const GLbyte* value;

		GLuint texIdx;
	};

	static unsigned int sm_idCount;
	MaterialID m_id;
	ShaderAsset_const m_shader;
	ShaderAttribFormat m_vertexFormat;
	std::vector<MatUniform> m_matUniforms;
	std::vector<ObjUniform> m_objUniforms;

	void initUniformData();
	void setUniform(GLenum type, size_t size, int uniform, GLuint texIdx, const void* value) const;
	void deleteUniformVal(const GLbyte* val, GLenum type);

public:
	Material(ShaderAsset_const shader, std::vector<Material::MaterialUniformInfo>& uniforms);

	inline void BindShader() const
	{
		glUseProgram(m_shader.get().getShader());
	}

	inline void BindMaterialParams() const
	{
		for (unsigned int n = 0; n < m_matUniforms.size(); n++)
		{
			setUniform(m_matUniforms[n].type, m_matUniforms[n].size, m_matUniforms[n].uniform, m_matUniforms[n].texIdx, m_matUniforms[n].value);
		}
	}

	inline void BindObjectParams(const MaterialObjectData& params) const
	{
		for (unsigned int n = 0; n < params.m_uniforms.size(); n++)
		{
			setUniform(m_objUniforms[n].type, m_objUniforms[n].size, m_objUniforms[n].uniform, m_objUniforms[n].texIdx, params.m_uniforms[n].value);
		}
	}

	inline MaterialObjectData newObjectParams() const
	{
		return MaterialObjectData(m_objUniforms, m_id);
	}

	const ShaderAttribFormat& getFormat() const;
	~Material();
};

typedef AssetKey<Material> MaterialKey;
typedef Asset<Material>	MaterialAsset;
typedef const Asset<Material> MaterialAsset_const;

template<>
class AssetKey<Material>
{
public:
	std::string m_name;
	std::string m_file;

	std::string toString() const
	{
		return "<Material>:" + m_name;
	}

	inline bool operator==(const MaterialKey& other) const
	{
		return m_name == other.m_name;
	}

	AssetKey(const std::string& name, const std::string& file) : m_name(name), m_file(file)
	{

	}
};

namespace std
{
	template <>
	struct hash<MaterialKey>
	{
		std::size_t operator()(const MaterialKey& k) const
		{
			return hash<std::string>()(k.m_name);
		}
	};
}

class MaterialAssetManager : public AssetSubManagerBase<Material>
{
private:
	Material* Load(MaterialKey key);
	void Unload(AssetIterator it);
public:
};