#pragma once

#include "EntityManager.h"
#include "AssetManager.h"
#include "BoundingBox.h"
#include "DataStructs.h"
#include "ei/glm/gtc/type_ptr.hpp"
#include <vector>

class Terrain
{
private:
	void makeMesh(unsigned int cells, float celllen, Mesh& mesh);

	unsigned int leafCells;
	float celllen;

	Mesh mesh;
	unsigned int singlePerm;
	unsigned int doublePerm;

	TextureAsset_const heightMap;
	TextureAsset_const grass;
	ShaderAsset_const shader;

	struct RenderInstance
	{
		unsigned short offx;
		unsigned short offz;
		unsigned short rotscale;
		unsigned short tex;
	};

	std::vector<RenderInstance> fullInstances;
	mutable unsigned int fullInstUsed;

	GLuint InstVBO;
	GLuint VAO;

public:
	Terrain() : leafCells(TerrainSettings::CellsPerLeaf), celllen(TerrainSettings::MinimumCellLength), mesh(0, 0, 0, 0, 0, 0), singlePerm(leafCells * leafCells * 6),
		doublePerm(singlePerm + (leafCells - 1) * leafCells * 6 + 9 * leafCells / 2), heightMap(AssetManager::getAsset<Texture>(TextureKey("heightmap.png"))), grass(AssetManager::getAsset<Texture>(TextureKey("grass.jpg"))),
		shader(AssetManager::getAsset<Shader>(ShaderKey("terrain.frag", "terrain.vert"))), fullInstUsed(0)
	{
		makeMesh(leafCells, celllen, mesh);

		glGenBuffers(1, &InstVBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertices());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, InstVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 4, GL_UNSIGNED_SHORT, 8, NULL);
		glVertexAttribDivisor(1, 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices());

		glBindVertexArray(0);
	}

	bool LoDCheck(AABoundingBox& aabb, unsigned int level, glm::lowp_uvec2& offset, const glm::vec3& camPos)
	{
		if (aabb.SqDistanceToPointXZ(camPos) >= TerrainSettings::LoDDistances[level] * TerrainSettings::LoDDistances[level])
		{
			if (fullInstances.size() <= fullInstUsed)
			{
				fullInstances.push_back(RenderInstance());
			}

			fullInstances[fullInstUsed].offx = offset.x;
			fullInstances[fullInstUsed].offz = offset.y;
			fullInstances[fullInstUsed].rotscale = level;
			fullInstances[fullInstUsed].tex = 0;

			fullInstUsed++;

			return true;
		}

		return false;
	}

	void Render(const glm::mat4& VP, glm::vec2 cam) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, InstVBO);
		glBufferData(GL_ARRAY_BUFFER, fullInstUsed * sizeof(RenderInstance), fullInstances.data(), GL_DYNAMIC_DRAW);

		glBindAttribLocation(shader.get().getShader(), 0, "in_Position");
		glBindAttribLocation(shader.get().getShader(), 1, "in_InstData");

		glLinkProgram(shader.get().getShader());

		glUseProgram(shader.get().getShader());

		GLbyte sp_WVP = glGetUniformLocation(shader.get().getShader(), "WVP");
		GLbyte sp_hmap = glGetUniformLocation(shader.get().getShader(), "hmap");
		GLbyte sp_gridsize = glGetUniformLocation(shader.get().getShader(), "gridSize");
		GLbyte sp_instbuff = glGetUniformLocation(shader.get().getShader(), "instBuff");
		GLbyte sp_grass = glGetUniformLocation(shader.get().getShader(), "grass");
		GLbyte sp_camPos = glGetUniformLocation(shader.get().getShader(), "camPos");

		glUniformMatrix4fv(sp_WVP, 1, GL_FALSE, glm::value_ptr(VP));
		glUniform1f(sp_gridsize, celllen * leafCells);
		glUniform2fv(sp_camPos, 1, glm::value_ptr(cam));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightMap.get().getTexture());
		glUniform1i(sp_hmap, 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, grass.get().getTexture());
		glUniform1i(sp_grass, 1);

		glBindVertexArray(VAO);

		//glDrawElements(GL_TRIANGLES, mesh.getIndlen() - doublePerm, GL_UNSIGNED_INT, (void*)(doublePerm * sizeof(unsigned int)));
		//glDrawElements(GL_TRIANGLES, doublePerm - singlePerm, GL_UNSIGNED_INT, (void*)(singlePerm * sizeof(unsigned int)));
		glDrawElementsInstanced(GL_TRIANGLES, singlePerm, GL_UNSIGNED_INT, (void*)(NULL), fullInstUsed);

		glBindVertexArray(0);

		fullInstUsed = 0;
	}

	~Terrain()
	{
		delete[] mesh.getIndBuff();
		delete[] mesh.getVertBuff();

		GLuint IBO = mesh.getIndices();
		GLuint VBO = mesh.getVertices();

		glDeleteBuffers(1, &IBO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &InstVBO);
	}
};