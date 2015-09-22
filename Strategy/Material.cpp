#include "Material.h"
#include "AssetManager.h"
#include <fstream>
#include <sstream>

unsigned int Material::sm_idCount = 0;

Material::Material(ShaderAsset_const shader, std::vector<Material::MaterialUniformInfo>& uniforms) : m_id(sm_idCount++), m_shader(shader), m_vertexFormat(shader)
{
	GLuint prog = m_shader.get().getShader();
	GLenum type, norType;
	GLsizei size, norSize;
	char name[512];

	GLint uniformCnt;
	glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &uniformCnt);

	char* isMatUni = new char[uniformCnt];
	memset(isMatUni, -1, sizeof(char)* uniformCnt);

	for (unsigned int n = 0; n < uniforms.size(); n++)
	{
		const char* str = uniforms[n].name.c_str();
		GLuint idx = 0;
		glGetUniformIndices(prog, 1, &str, &idx);

		if (idx == GL_INVALID_INDEX)
		{
			WARNING(("Material uniform '" + uniforms[n].name + "' doesn´t exist").c_str());
			delete[] uniforms[n].value;
			continue;
		}

		isMatUni[idx] = n;
	}

	GLuint texCounter = 0;

	for (int n = 0; n < uniformCnt; n++)
	{
		glGetActiveUniform(prog, n, 512, 0, &size, &type, name);

		if (isMatUni[n] >= 0)
		{
			norType = type;
			norSize = size;

			NormalizeType(norType, norSize);

			if (norType != uniforms[isMatUni[n]].type || norSize != uniforms[isMatUni[n]].size)
			{
				WARNING(("Type mismatch for material uniform '" + uniforms[isMatUni[n]].name + "'").c_str());
				delete[] uniforms[isMatUni[n]].value;
				uniforms[isMatUni[n]].value = NULL;
				continue;
			}

			m_matUniforms.push_back(MatUniform());
			m_matUniforms.back().size = size;
			m_matUniforms.back().type = type;
			m_matUniforms.back().value = uniforms[isMatUni[n]].value;
			m_matUniforms.back().uniform = glGetUniformLocation(prog, name);
			m_matUniforms.back().texIdx = 0;

			if (type == GL_SAMPLER_1D || type == GL_SAMPLER_2D || type == GL_SAMPLER_3D)
			{
				m_matUniforms.back().texIdx = texCounter++;
			}
		}
		else
		{
			m_objUniforms.push_back(ObjUniform());
			m_objUniforms.back().size = size;
			m_objUniforms.back().type = type;
			m_objUniforms.back().uniform = glGetUniformLocation(prog, name);
			m_objUniforms.back().name = name;
			m_objUniforms.back().value = NULL;
			m_objUniforms.back().texIdx = 0;

			if (type == GL_SAMPLER_1D || type == GL_SAMPLER_2D || type == GL_SAMPLER_3D)
			{
				m_objUniforms.back().texIdx = texCounter++;
			}
		}
	}

	delete[] isMatUni;
}

void Material::deleteUniformVal(const GLbyte* val, GLenum type)
{
	switch (type)
	{
	case GL_FLOAT:
		delete[] reinterpret_cast<const float*>(val);
		break;
	case GL_INT:
		delete[] reinterpret_cast<const int*>(val);
		break;
	case GL_UNSIGNED_INT:
		delete[] reinterpret_cast<const unsigned int*>(val);
		break;
	case GL_DOUBLE:
		delete[] reinterpret_cast<const double*>(val);
		break;
	case GL_BOOL:
		delete[] reinterpret_cast<const bool*>(val);
		break;
	case GL_SAMPLER_1D:
		delete reinterpret_cast<TextureAsset_const*>(val);
	}
}

Material::~Material()
{
	for (unsigned int n = 0; n < m_matUniforms.size(); n++)
	{
		deleteUniformVal(m_matUniforms[n].value, m_matUniforms[n].type);
	}
}

void Material::setUniform(GLenum type, size_t size, int uniform, GLuint texIdx, const void* value) const
{
	GLuint tex;

	switch (type)
	{
	case GL_FLOAT:
		glUniform1fv(uniform, size, (const GLfloat*)value);
		break;
	case GL_FLOAT_VEC2:
		glUniform2fv(uniform, size, (const GLfloat*)value);
		break;
	case GL_FLOAT_VEC3:
		glUniform3fv(uniform, size, (const GLfloat*)value);
		break;
	case GL_FLOAT_VEC4:
		glUniform4fv(uniform, size, (const GLfloat*)value);
		break;
	case GL_DOUBLE:
		glUniform1dv(uniform, size, (const GLdouble*)value);
		break;
	case GL_DOUBLE_VEC2:
		glUniform2dv(uniform, size, (const GLdouble*)value);
		break;
	case GL_DOUBLE_VEC3:
		glUniform3dv(uniform, size, (const GLdouble*)value);
		break;
	case GL_DOUBLE_VEC4:
		glUniform4dv(uniform, size, (const GLdouble*)value);
		break;
	case GL_INT:
		glUniform1iv(uniform, size, (const GLint*)value);
		break;
	case GL_INT_VEC2:
		glUniform2iv(uniform, size, (const GLint*)value);
		break;
	case GL_INT_VEC3:
		glUniform3iv(uniform, size, (const GLint*)value);
		break;
	case GL_INT_VEC4:
		glUniform4iv(uniform, size, (const GLint*)value);
		break;
	case GL_UNSIGNED_INT:
		glUniform1uiv(uniform, size, (const GLuint*)value);
		break;
	case GL_UNSIGNED_INT_VEC2:
		glUniform2uiv(uniform, size, (const GLuint*)value);
		break;
	case GL_UNSIGNED_INT_VEC3:
		glUniform3uiv(uniform, size, (const GLuint*)value);
		break;
	case GL_UNSIGNED_INT_VEC4:
		glUniform4uiv(uniform, size, (const GLuint*)value);
		break;
	case GL_BOOL:
		glUniform1iv(uniform, size, (const GLint*)value);
		break;
	case GL_BOOL_VEC2:
		glUniform2iv(uniform, size, (const GLint*)value);
		break;
	case GL_BOOL_VEC3:
		glUniform3iv(uniform, size, (const GLint*)value);
		break;
	case GL_BOOL_VEC4:
		glUniform4iv(uniform, size, (const GLint*)value);
		break;
	case GL_FLOAT_MAT2:
		glUniformMatrix2fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT3:
		glUniformMatrix3fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT4:
		glUniformMatrix4fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT2x3:
		glUniformMatrix2x3fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT2x4:
		glUniformMatrix2x4fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT3x2:
		glUniformMatrix3x2fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT3x4:
		glUniformMatrix3x4fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT4x2:
		glUniformMatrix4x2fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_FLOAT_MAT4x3:
		glUniformMatrix4x3fv(uniform, size, GL_FALSE, (const GLfloat*)value);
		break;
	case GL_SAMPLER_1D:
		tex = reinterpret_cast<TextureAsset_const*>(value)->get().getTexture();
		glActiveTexture(GL_TEXTURE0 + texIdx);
		glBindTexture(GL_TEXTURE_1D, tex);
		glUniform1i(uniform, texIdx);
	case GL_SAMPLER_2D:
		tex = reinterpret_cast<TextureAsset_const*>(value)->get().getTexture();
		glActiveTexture(GL_TEXTURE0 + texIdx);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform, texIdx);
	case GL_SAMPLER_3D:
		tex = reinterpret_cast<TextureAsset_const*>(value)->get().getTexture();
		glActiveTexture(GL_TEXTURE0 + texIdx);
		glBindTexture(GL_TEXTURE_3D, tex);
		glUniform1i(uniform, texIdx);
		break;
	default:
		assert(0);
	}
}

const ShaderAttribFormat& Material::getFormat() const
{
	return m_vertexFormat;
}

bool MaterialObjectData::checkTypeMatching(UniformID uniform, GLenum type, GLint size) const
{
#ifndef MATERIAL_TYPE_UNSAFE
	NormalizeType(type, size);
	
	GLenum norType = m_uniforms[uniform].type;
	GLint norSize = m_uniforms[uniform].size;

	NormalizeType(norType, norSize);

	return type == norType && size == norSize;
#else
	return true;
#endif
}

MaterialObjectData::MaterialObjectData(const std::vector<ObjUniform>& uniforms, MaterialID parent) : m_uniforms(uniforms), m_parent(parent)
{
	for (unsigned int n = 0; n < m_uniforms.size(); n++)
	{
		m_uniforms[n].value = new GLbyte[getShaderTypeSize(m_uniforms[n].type)];
	}
}

MaterialObjectData::MaterialObjectData(const MaterialObjectData& other) : m_uniforms(other.m_uniforms), m_parent(other.m_parent)
{
	for (unsigned int n = 0; n < m_uniforms.size(); n++)
	{
		m_uniforms[n].value = new GLbyte[getShaderTypeSize(m_uniforms[n].type)];
		memcpy(m_uniforms[n].value, other.m_uniforms[n].value, getShaderTypeSize(m_uniforms[n].type));
	}
}

MaterialObjectData::MaterialObjectData(MaterialObjectData&& other) : m_uniforms(std::move(other.m_uniforms)), m_parent(other.m_parent)
{

}

const MaterialObjectData& MaterialObjectData::operator=(const MaterialObjectData& rhs)
{
	for (unsigned int n = 0; n < m_uniforms.size(); n++)
	{
		delete[] m_uniforms[n].value;
		m_uniforms[n] = rhs.m_uniforms[n];
		m_uniforms[n].value = new GLbyte[getShaderTypeSize(m_uniforms[n].type)];
		memcpy(m_uniforms[n].value, rhs.m_uniforms[n].value, getShaderTypeSize(m_uniforms[n].type));
	}

	return *this;
}

MaterialObjectData::~MaterialObjectData()
{
	for (unsigned int n = 0; n < m_uniforms.size(); n++)
	{
		delete[] m_uniforms[n].value;
	}
}

UniformID MaterialObjectData::checkUniform(const std::string& uniform) const
{
	for (unsigned int n = 0; n < m_uniforms.size(); n++)
	{
		if (m_uniforms[n].name.compare(uniform) == 0)
		{
			return n;
		}
	}

	return -1;
}

template<typename T>
bool readValues(std::stringstream& strstr, Material::MaterialUniformInfo& current, std::string& errstr)
{
	T *data = new T[current.size];

	for (unsigned int n = 0; n < current.size; n++)
	{
		if (!(strstr >> data[n]))
		{
			errstr = "Values specified for uniform '" + current.name + "' can't be converted to requested type";
			return false;
		}
	}

	current.value = reinterpret_cast<const GLbyte*>(data);
	return true;
}

template<>
bool readValues<bool>(std::stringstream& strstr, Material::MaterialUniformInfo& current, std::string& errstr)
{
	bool* data = new bool[current.size];

	for (unsigned int n = 0; n < current.size; n++)
	{
		if (!(strstr >> std::boolalpha >> data[n]))
		{
			errstr = "Values specified for uniform '" + current.name + "' can't be converted to requested type";
			return false;
		}
	}

	current.value = reinterpret_cast<const GLbyte*>(data);
	return true;
}

template<>
bool readValues<TextureAsset_const>(std::stringstream& strstr, Material::MaterialUniformInfo& current, std::string& errstr)
{
	if (current.size != 1)
	{
		errstr = "Invalid type for uniform '" + current.name + "'. Texture arrays not supported";
		return false;
	}

	std::string name;
	if (!(strstr >> name))
	{
		errstr = "Values specified for uniform '" + current.name + "' can't be converted to requested type";
		return false;
	}

	TextureAsset_const* data = new TextureAsset_const(AssetManager::getAsset<Texture>(TextureKey(name)));

	current.value = reinterpret_cast<const GLbyte*>(data);
	return true;
}

typedef bool(*ReadFunc)(std::stringstream&, Material::MaterialUniformInfo&, std::string&);

Material* MaterialAssetManager::Load(MaterialKey key)
{
	std::ifstream file(key.m_file);

	if (!file)
	{
		throw AssetLoadException<Material>(key, "Couldn't open file!");
	}

	file.seekg(0, std::ios::end);
	size_t size = (size_t)file.tellg();
	std::string str;
	str.resize(size);
	file.seekg(0);
	file.read(&str[0], size);
	file.close();

	std::stringstream strstr(str);

	enum State { ST_NOTFOUND, ST_INVALID, ST_FINISHED, ST_MAT, ST_FRAG, ST_VERT, ST_UNIFORMNAME, ST_UNIFORMTYPE, ST_UNIFORMVALUE, ST_MATNAME };
	const char* typeStrings[] = { "float", "double", "int", "uint", "bool", "texture"};
	size_t typeLens[] = { strlen(typeStrings[0]), strlen(typeStrings[1]), strlen(typeStrings[2]), strlen(typeStrings[3]), strlen(typeStrings[4]), strlen(typeStrings[5]) };
	GLenum typeEnum[] = { GL_FLOAT, GL_DOUBLE, GL_INT, GL_UNSIGNED_INT, GL_BOOL, GL_SAMPLER_1D };
	ReadFunc readFncs[] = { readValues<float>, readValues<double>, readValues<int>, readValues<unsigned int>, readValues<bool>, readValues<TextureAsset_const> };
	const int typeCount = 6;

	State state = ST_NOTFOUND;

	std::string frag(""), vert("");
	Material::MaterialUniformInfo current;
	std::vector<Material::MaterialUniformInfo> uniforms;

	std::string errstr = "";			
	ReadFunc fnc = NULL;

	while (strstr >> str && state != ST_FINISHED && state != ST_INVALID)
	{
		if (!str.length() || !str[0])
		{
			continue;
		}

		switch (state)
		{
		case ST_NOTFOUND:
			if (str.compare("Mat") == 0)
			{
				state = ST_MATNAME;
			}
			break;
		case ST_MATNAME:
			if (str.compare(key.m_name) == 0)
			{
				state = ST_MAT;
			}
			else
			{
				state = ST_NOTFOUND;
			}
			break;
		case ST_MAT:
			if (str.compare("-frag") == 0)
			{
				state = ST_FRAG;
			}
			else if (str.compare("-vert") == 0)
			{
				state = ST_VERT;
			}
			else if (str.compare("-uniform") == 0)
			{
				state = ST_UNIFORMNAME;
			}
			else if (str.compare("Mat") == 0)
			{
				state = ST_FINISHED;
			}
			else if (str.length() < 2 || str[0] != '/' || str[1] != '/')
			{
				state = ST_INVALID;
				errstr = "'" + str + "' is not a valid keyword in a material script body.";
			}
			else
			{
				std::getline(strstr, str);
			}
			break;
		case ST_FRAG:
			frag = str;
			state = ST_MAT;
			break;
		case ST_VERT:
			vert = str;
			state = ST_MAT;
			break;
		case ST_UNIFORMNAME:
			current.name = str;
			state = ST_UNIFORMTYPE;
			break;
		case ST_UNIFORMTYPE:
			state = ST_INVALID;

			for (unsigned int n = 0; n < typeCount; n++)
			{
				if (str.compare(0, typeLens[n], typeStrings[n]) == 0)
				{
					current.type = typeEnum[n];
					fnc = readFncs[n];

					if (str.length() - typeLens[n] == 0)
					{
						current.size = 1;
					}
					else	//Vector type
					{
						unsigned int arrLen = 0;
						unsigned int e = 1;
						for (unsigned int s = str.length() - 1; s >= typeLens[n]; s--)
						{
							if (str[s] < '0' || str[s] > '9')
							{
								arrLen = 0;
								break;
							}

							arrLen += (str[s] - '0') * e;
							e *= 10;
						}

						if (!arrLen)
						{
							break;
						}

						current.size = arrLen;
					}

					state = ST_UNIFORMVALUE;
					break;
				}
			}

			if (state == ST_INVALID)
			{
				errstr = "'" + str + "' is not a valid uniform type.";
				break;
			}
		//ST_UNIFORMVALUE
			
			if (!fnc(strstr, current, errstr))
			{
				state = ST_INVALID;
			}
			else
			{
				state = ST_MAT;
				uniforms.push_back(current);
			}

			break;
		default:
			throw AssetLoadException<Material>(key, "Material parser bug occured!");
			break;
		}
	}

	if (state == ST_INVALID)
	{
		throw AssetLoadException<Material>(key, "Material parsing Error: " + errstr);
	}

	if (state == ST_NOTFOUND)
	{
		throw AssetLoadException<Material>(key, "File '" + key.m_file + "' doesn't contain a Material called '" + key.m_name + "'.");
	}

	if (!frag.length() || !vert.length())
	{
		throw AssetLoadException<Material>(key, "Material parsing Error: No fragment- or vertexshader specified");
	}


	ShaderAsset_const shader = AssetManager::getAsset<Shader>(ShaderKey(frag, vert));

	return new Material(shader, uniforms);
}

void MaterialAssetManager::Unload(AssetIterator it)
{

}

//Shader* ShaderAssetManager::Load(ShaderKey key)
//{
//	SubShaderAsset_const frag = AssetManager::getAsset(SubShaderKey(key.m_frag, true));
//	SubShaderAsset_const vert = AssetManager::getAsset(SubShaderKey(key.m_vert, false));
//	GLuint prog = glCreateProgram();
//	glAttachShader(prog, frag.get().getShader());
//	glAttachShader(prog, vert.get().getShader());
//	glLinkProgram(prog);
//
//	return new Shader(prog, frag, vert);
//}
//
//void ShaderAssetManager::Unload(AssetIterator it)
//{
//	glDeleteProgram(it->second.m_pData->getShader());
//}