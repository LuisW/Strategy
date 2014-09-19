#include "TerrainGenerator.h"
#include "Map.h"
#include "DataStructs.h"

unsigned int crand(unsigned int seed)
{
	seed += 10;
	seed = (seed ^ 61u) ^ (seed >> 16u);
	seed *= 9u;
	seed = seed ^ (seed >> 4u);
	seed *= 668265261u;
	return seed ^ (seed >> 15u);
}

#define hmapres TerrainSettings::HeightMapRes

GLuint TerrainGenerator::getHeightMap() const
{
	unsigned short* data = new unsigned short[hmapres * (hmapres + 1)];
	memset(data, 0, sizeof(unsigned short) * hmapres * (hmapres + 1));

	GLuint tex = 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16, hmapres, hmapres);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, hmapres, hmapres, GL_RED, GL_UNSIGNED_SHORT, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete[] data;

	return tex;
}