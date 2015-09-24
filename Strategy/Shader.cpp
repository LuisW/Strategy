#include "Shader.h"
#include "AssetManager.h"
#include "MaterialTypeMap.h"

Shader* ShaderAssetManager::Load(ShaderKey key)
{
	SubShaderAsset_const frag = AssetManager::getAsset(SubShaderKey(key.m_frag, true));
	SubShaderAsset_const vert = AssetManager::getAsset(SubShaderKey(key.m_vert, false));
	GLuint prog = glCreateProgram();
	glAttachShader(prog, frag.get().getShader());
	glAttachShader(prog, vert.get().getShader());

	glLinkProgram(prog);

	GLint loc;
	GLint count;
	GLsizei len;
	GLint size;
	GLenum type;
	GLchar name[512];
	unsigned int semIdx;
	unsigned int addAttribCnt = 8;

	glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &count);
	for (int n = 0; n < count; n++)
	{
		glGetActiveAttrib(prog, n, 512, &len, &size, &type, name);

		for (unsigned int i = 0; i <= SEM_USER; i++)
		{
			if (strncmp(name, SemanticNames[i], SemNameLen[i]) == 0)
			{
				char* idx = name + SemNameLen[i];
				if (*idx)
				{
					sscanf_s(idx, "%i", &(semIdx));
				}
				else
				{
					semIdx = 0;
				}

				GLuint loc = addAttribCnt;

				// Position0 -> 0, Normal0 -> 1, Tangent0 -> 2, Color0 -> 3, TexCoord0 -> 4, TexCoord1 -> 5, Weight0 -> 6, Weight1 -> 7, Rest ascending from 8.
				switch (i)
				{
				case SEM_POSITION:
					if (semIdx == 0)
						loc = 0;
					break;
				case SEM_NORMAL:
					if (semIdx == 0)
						loc = 1;
					break;
				case SEM_TANGENT:
					if (semIdx == 0)
						loc = 2;
					break;
				case SEM_COLOR:
					if (semIdx == 0)
						loc = 3;
					break;
				case SEM_TEXCOORD:
					if (semIdx <= 1)
						loc = 4 + semIdx;
					break;
				case SEM_WEIGHT:
					if (semIdx <= 1)
						loc = 6 + semIdx;
					break;
				}

				if (loc > 7)
					addAttribCnt++;

				glBindAttribLocation(prog, loc, name);
				break;
			}

			if (SEM_USER == i)
			{
				glBindAttribLocation(prog, addAttribCnt++, name);
			}
		}
	}

	glLinkProgram(prog);

	return new Shader(prog, frag, vert);
}

void ShaderAssetManager::Unload(AssetIterator it)
{
	glDeleteProgram(it->second.m_pData->getShader());
}