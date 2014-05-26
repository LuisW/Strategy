#pragma once

#include "EntityManager.h"
#include "AssetManager.h"
#include "BoundingBox.h"
#include "DataStructs.h"
#include "ei/glm/gtc/type_ptr.hpp"
#include <vector>

enum ChildLocation { CHILD_NW, CHILD_NE, CHILD_SW, CHILD_SE };

class Terrain
{
private:
	void makeMesh(unsigned int cells, float celllen, Mesh& mesh);

	inline float SqDistanceToPointXZ(const glm::vec3& min, float lenOver2, const glm::vec3& p)
	{
		float dx = glm::max(glm::abs(p.x - min.x - lenOver2) - lenOver2, 0.0f);
		float dz = glm::max(glm::abs(p.z - min.z - lenOver2) - lenOver2, 0.0f);

		return dx*dx + dz*dz;
	}

	inline float LevelLen(unsigned int level)
	{
		return TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength * (float)(1 << level);
	}

	unsigned int leafCells;
	float celllen;

	Mesh mesh;

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

	struct TerrainMesh
	{
		std::vector<RenderInstance> Instances;
		mutable unsigned int InstUsed;
		unsigned int offset;
		unsigned int len;
	};

	TerrainMesh full;
	TerrainMesh singlePerm;
	TerrainMesh doublePerm;

	GLuint InstVBO;
	GLuint VAO;

public:
	Terrain() : leafCells(TerrainSettings::CellsPerLeaf), celllen(TerrainSettings::MinimumCellLength), mesh(0, 0, 0, 0, 0, 0), heightMap(AssetManager::getAsset<Texture>(TextureKey("heightmap.png"))),
		grass(AssetManager::getAsset<Texture>(TextureKey("grass.jpg"))), shader(AssetManager::getAsset<Shader>(ShaderKey("terrain.frag", "terrain.vert")))
	{
		makeMesh(leafCells, celllen, mesh);

		singlePerm.InstUsed = doublePerm.InstUsed = full.InstUsed = 0;
		full.offset = 0;
		full.len = singlePerm.offset = leafCells * leafCells * 6;
		singlePerm.len = (leafCells - 1) * leafCells * 6 + 9 * leafCells / 2;
		doublePerm.offset = singlePerm.offset + singlePerm.len;
		doublePerm.len = mesh.getIndlen() - doublePerm.offset;

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

	bool LoDCheck(AABoundingBox& aabb, unsigned int level, glm::lowp_uvec2& offset, const glm::vec3& camPos, ChildLocation loc)
	{
		if (SqDistanceToPointXZ(aabb.getMin(), LevelLen(level) * 0.5f, camPos) >= TerrainSettings::LoDDistances[level] * TerrainSettings::LoDDistances[level])
		{
			bool Dist1, Dist2;
			unsigned short rot;

			switch (loc)
			{
			case CHILD_SW:
				//W
				Dist1 = SqDistanceToPointXZ(aabb.getMin() - glm::vec3(LevelLen(level + 1), 0.0f, LevelLen(level + 1)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//S
				Dist2 = SqDistanceToPointXZ(aabb.getMin() + glm::vec3(0.0f, 0.0f, LevelLen(level)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				if (!Dist1 && !Dist2)
				{
					rot = 4;
				}
				else if (Dist1 && !Dist2)
				{
					rot = 0;
				}
				else if (!Dist1 && Dist2)
				{
					rot = 3;
				}
				else
				{
					rot = 3;
				}

				break;
			case CHILD_SE:
				//E
				Dist1 = SqDistanceToPointXZ(aabb.getMin() + glm::vec3(LevelLen(level), 0.0f, -LevelLen(level)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//S
				Dist2 = SqDistanceToPointXZ(aabb.getMin() + glm::vec3(-LevelLen(level), 0.0f, LevelLen(level)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				if (!Dist1 && !Dist2)
				{
					rot = 4;
				}
				else if (Dist1 && !Dist2)
				{
					rot = 2;
				}
				else if (!Dist1 && Dist2)
				{
					rot = 3;
				}
				else
				{
					rot = 2;
				}

				break;
			case CHILD_NW:
				//W
				Dist1 = SqDistanceToPointXZ(aabb.getMin() - glm::vec3(LevelLen(level + 1), 0.0f, 0.0f), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//N
				Dist2 = SqDistanceToPointXZ(aabb.getMin() - glm::vec3(0.0f, 0.0f, LevelLen(level + 1)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				if (!Dist1 && !Dist2)
				{
					rot = 4;
				}
				else if (Dist1 && !Dist2)
				{
					rot = 0;
				}
				else if (!Dist1 && Dist2)
				{
					rot = 1;
				}
				else
				{
					rot = 0;
				}

				break;
			case CHILD_NE:
				//E
				Dist1 = SqDistanceToPointXZ(aabb.getMin() + glm::vec3(LevelLen(level), 0.0f, 0.0f), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//N
				Dist2 = SqDistanceToPointXZ(aabb.getMin() + glm::vec3(-LevelLen(level), 0.0f, -LevelLen(level + 1)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				if (!Dist1 && !Dist2)
				{
					rot = 4;
				}
				else if (Dist1 && !Dist2)
				{
					rot = 2;
				}
				else if (!Dist1 && Dist2)
				{
					rot = 1;
				}
				else
				{
					rot = 1;
				}

				break;
			}

			if (rot == 4)
			{
				if (full.Instances.size() <= full.InstUsed)
				{
					full.Instances.push_back(RenderInstance());
				}

				full.Instances[full.InstUsed].offx = offset.x;
				full.Instances[full.InstUsed].offz = offset.y;
				full.Instances[full.InstUsed].rotscale = level;
				full.Instances[full.InstUsed].tex = 0;

				full.InstUsed++;
			}
			else if (Dist1 && Dist2)
			{
				if (doublePerm.Instances.size() <= doublePerm.InstUsed)
				{
					doublePerm.Instances.push_back(RenderInstance());
				}

				doublePerm.Instances[doublePerm.InstUsed].offx = offset.x;
				doublePerm.Instances[doublePerm.InstUsed].offz = offset.y;
				doublePerm.Instances[doublePerm.InstUsed].rotscale = level | (rot << 8);
				doublePerm.Instances[doublePerm.InstUsed].tex = 0;

				doublePerm.InstUsed++;
			}
			else
			{
				if (singlePerm.Instances.size() <= singlePerm.InstUsed)
				{
					singlePerm.Instances.push_back(RenderInstance());
				}

				singlePerm.Instances[singlePerm.InstUsed].offx = offset.x;
				singlePerm.Instances[singlePerm.InstUsed].offz = offset.y;
				singlePerm.Instances[singlePerm.InstUsed].rotscale = level | (rot << 8);
				singlePerm.Instances[singlePerm.InstUsed].tex = 0;

				singlePerm.InstUsed++;
			}
			

			return true;
		}

		return false;
	}

	void Render(const glm::mat4& VP, glm::vec2 cam) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, InstVBO);
		glBufferData(GL_ARRAY_BUFFER, (full.InstUsed + singlePerm.InstUsed + doublePerm.InstUsed) * sizeof(RenderInstance), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, full.InstUsed * sizeof(RenderInstance), full.Instances.data());
		glBufferSubData(GL_ARRAY_BUFFER, full.InstUsed * sizeof(RenderInstance), singlePerm.InstUsed * sizeof(RenderInstance), singlePerm.Instances.data());
		glBufferSubData(GL_ARRAY_BUFFER, (singlePerm.InstUsed + full.InstUsed) * sizeof(RenderInstance), doublePerm.InstUsed * sizeof(RenderInstance), doublePerm.Instances.data());

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

		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, full.len      , GL_UNSIGNED_INT, (void*)(full.offset       * sizeof(unsigned int)), full.InstUsed      , 0);
		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, singlePerm.len, GL_UNSIGNED_INT, (void*)(singlePerm.offset * sizeof(unsigned int)), singlePerm.InstUsed, full.InstUsed);
		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, doublePerm.len, GL_UNSIGNED_INT, (void*)(doublePerm.offset * sizeof(unsigned int)), doublePerm.InstUsed, full.InstUsed + singlePerm.InstUsed);

		glBindVertexArray(0);

		singlePerm.InstUsed = doublePerm.InstUsed = full.InstUsed = 0;
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