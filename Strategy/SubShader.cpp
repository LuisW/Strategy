#include "SubShader.h"
#include <fstream>

SubShader* SubShaderAssetManager::Load(SubShaderKey key)
{
	GLenum type = GL_VERTEX_SHADER;
	if (key.m_isfrag)
	{
		type = GL_FRAGMENT_SHADER;
	}

	GLuint shader = glCreateShader(type);

	std::ifstream file(key.m_name);

	if (!file)
	{
		throw AssetLoadException<SubShader>(key, "Couldn't open file!");
	}

	file.seekg(0, std::ios::end);
	size_t size = (size_t)file.tellg();
	std::string code;
	code.resize(size);
	file.seekg(0);
	file.read(&code[0], size);
	file.close();

	const char* str = code.c_str();

	glShaderSource(shader, 1, &str, (const GLint*)(&size));
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint logsize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
		char* log = new char[logsize];
		glGetShaderInfoLog(shader, logsize, &logsize, log);

		printf(log);

		delete[] log;
		glDeleteShader(shader);
		throw AssetLoadException<SubShader>(key, "Shader compilation error!");
	}

	return new SubShader(shader, key.m_isfrag);
}

void SubShaderAssetManager::Unload(SubShaderAssetManager::AssetIterator it)
{
	glDeleteShader(it->second.m_pData->getShader());
}