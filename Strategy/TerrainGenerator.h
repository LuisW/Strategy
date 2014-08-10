#pragma once

#include "DataStructs.h"
#include "AssetManager.h"

class TerrainGenerator
{
private:
	std::vector<GLuint> hMaps;
	ShaderAsset_const shader;
	Mesh quad;

public:
	TerrainGenerator() : shader(AssetManager::getAsset<Shader>(ShaderKey("HMapGen.frag", "HMapGen.vert"))), quad(0,0,0,0,0,0)
	{
		GLuint IBO, VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		
		float verts[] = { -1.0f, -1.0f, 0.5f, 1.0f, -1.0f, 0.5f, 1.0f, 1.0f, 0.5f, -1.0f, 1.0f, 0.5f };
		unsigned int inds[] = { 0, 1, 2, 0, 2, 3 };
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), verts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), inds, GL_STATIC_DRAW);

		quad = Mesh(VBO, IBO, 12, 6, NULL, NULL);
	}

	GLuint getHeightMap() const;
	

	~TerrainGenerator()
	{
		quad.clear();
	}
};