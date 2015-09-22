#pragma once

#include "DataStructs.h"
#include "AssetManager.h"
#include "ProcGenHelp.h"

#define hmapres ((TerrainSettings::HeightMapRes - 1) * TerrainSettings::HeightMapCnt + 1)

class TerrainGenerator
{
private:
	Mesh Quad;
	GLuint VAO;
	GLuint tex;
	SDL_Surface* surf;
	int chunksize;
	unsigned int chunkcnt;
	Uint16* heightmap;

	static const unsigned int tileCnt = (1 << ((2 *TerrainSettings::LoDs + 2) - 1)) / 3; //  ( 4^(n+1) - 1 ) / 3

	Uint16* getHeightmapAt(unsigned int x, unsigned int y)
	{
		return heightmap + y * hmapres + x;
	}

	TerrainGenerator(const TerrainGenerator& other){}
	const TerrainGenerator& operator=(const TerrainGenerator& rhs){ return *this; }

	struct TileNode
	{

	};

public:
	TerrainGenerator()
	{
		//***** Generate fullscreen render quad *****
		float verts[] =
		{-1.0f,-1.0f, 0.5f,
		  1.0f,-1.0f, 0.5f,
		  1.0f, 1.0f, 0.5f,
		 -1.0f, 1.0f, 0.5f};

		unsigned int inds[] = { 0, 1, 2, 0, 2, 3 };

		GLuint BO[2] = { 0 };
		glGenBuffers(2, BO);
		glBindBuffer(GL_ARRAY_BUFFER, BO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 12, verts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, inds, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, BO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BO[1]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 12, NULL);
		glVertexAttribDivisor(1, 0);

		glBindVertexArray(0);

		VertexAttribFormat fmt(2 * sizeof(float));
		fmt.addElem(0, GL_FLOAT, 2, SEM_POSITION, 0);

		//Quad = Mesh(BO[0], BO[1], 12, 6, NULL, NULL, fmt);

		//***** Generate rendertarget texture *****

		int maxTexSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
		chunksize = 512;
		while ((chunksize << 1) + 1 < maxTexSize)
		{
			chunksize <<= 1;
		}

		if (chunksize >= hmapres - 1)
		{
			chunksize = hmapres;
			chunkcnt = 1;
		}
		else
		{
			chunkcnt = (hmapres - 1) / chunksize;
			chunksize++;
		}


		surf = SDL_AllocSurface(SDL_SWSURFACE, chunksize, chunksize, 16, 0xFFFF, 0x0, 0x0, 0x0);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16, chunksize, chunksize);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, chunksize, chunksize, GL_RED, GL_UNSIGNED_SHORT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	void GenerateHeightMap(unsigned int seed, glm::uvec2 pos);

	void GenerateFullTerrain();

	~TerrainGenerator()
	{
		Quad.clear();
		glDeleteTextures(1, &tex);
		SDL_FreeSurface(surf);
	}
};