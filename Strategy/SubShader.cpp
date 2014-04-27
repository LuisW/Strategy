#include "SubShader.h"
#include <fstream>

SubShader* SubShaderAssetManager::Load(SubShaderKey key)
{
	GLenum type = GL_VERTEX_SHADER;
	if (key.isfrag)
	{
		type = GL_FRAGMENT_SHADER;
	}

	GLuint shader = glCreateShader(type);

	try
	{
		std::ifstream file(key.name);

		if (!file)
		{
			return NULL;
		}

		file.seekg(0, std::ios::end);
		size_t size = (size_t)file.tellg();
		std::string code(size, ' ');
		file.seekg(0);
		file.read(&code[0], size);

		const char* str = code.c_str();

		glShaderSource(shader, 1, &str, (const GLint*)(&size));
		glCompileShader(shader);

		return new SubShader(shader, key.isfrag);
	}
	catch (std::exception e)
	{
		return NULL;
	}
}

void SubShaderAssetManager::Unload(SubShaderAssetManager::AssetIterator it)
{
	glDeleteShader(it->second.data->getShader());
}