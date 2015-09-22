#include "TerrainGenerator.h"
#include "DataStructs.h"
#include "ProcGenHelp.h"
#include <fstream>

void TerrainGenerator::GenerateHeightMap(unsigned int seed, glm::uvec2 pos)
{
	ShaderAsset_const shader = AssetManager::getAsset<Shader>(ShaderKey("HMapGen.frag", "HMapGen.vert"));

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, chunksize, chunksize);

	glBindTexture(GL_TEXTURE_2D, tex);

	GLuint FBO = 0;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, tex);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	glBindAttribLocation(shader.get().getShader(), 0, "in_Position");
	glBindFragDataLocation(shader.get().getShader(), 0, "out_Height");
	glLinkProgram(shader.get().getShader());
	glUseProgram(shader.get().getShader());

	GLbyte sp_off = glGetUniformLocation(shader.get().getShader(), "off");

	glBindVertexArray(VAO);

	glUniform2ui(sp_off, pos.x, pos.y);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FBO);

	glPopAttrib();

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_SHORT, surf->pixels);

	for (int y = 0; y < chunksize; y++)
	{
		memcpy(getHeightmapAt(pos.x, pos.y + y), (Uint8*)(surf->pixels) + surf->pitch * y, surf->format->BytesPerPixel * chunksize);
	}
}

void TerrainGenerator::GenerateFullTerrain()
{
	heightmap = new Uint16[chunksize * chunksize];

	glm::vec2 mapCenter = randPoint(glm::vec2(0.0f), 0.05f, 0);
	glm::vec3 city1 = glm::vec3(randPoint(glm::vec2(0.8f), 0.05f, 0), 0.13f);
	glm::vec3 city2 = glm::vec3(randPoint(glm::vec2(0.0f, -0.8f), 0.05f, 0), 0.13f);
	glm::vec3 city3 = glm::vec3(randPoint(glm::vec2(-0.8f, 0.8f), 0.05f, 0), 0.13f);

	unsigned int tileTable[tileCnt];
	long filePos = 0;

	for (unsigned int x = 0; x < chunkcnt; x++)
	{
		for (unsigned int y = 0; y < chunkcnt; y++)
		{
			GenerateHeightMap(x + chunkcnt * y, glm::uvec2(x*(chunksize-1), y*(chunksize-1)));

			std::ofstream hmapFile;
			hmapFile.open("Test.hmap", std::ios::binary | std::ios::out);
			if(hmapFile.is_open())
			{
				hmapFile.write((const char*)heightmap, sizeof(Uint16)* hmapres * hmapres);
			}
			hmapFile.close();
		}
	}

	delete[] heightmap;
}

