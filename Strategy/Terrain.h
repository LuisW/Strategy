#pragma once

#include "EntityManager.h"
#include "AssetManager.h"
#include "BoundingBox.h"
#include "DataStructs.h"
#include "ei/glm/gtc/type_ptr.hpp"
#include <vector>
#include "HeightmapManager.h"

class Terrain
{
private:
	void makeMesh(unsigned int cells, float celllen, Mesh& mesh);

	Mesh m_mesh;

	TextureAsset_const	m_grass;
	TextureAsset_const	m_rock;
	ShaderAsset_const	m_shader;

	struct RenderInstance
	{
		unsigned short offx;
		unsigned short offz;
		unsigned short rotscale;
		unsigned short tex;
		//float lodLerp;
	};

	struct TerrainMesh
	{
		std::vector<RenderInstance> instances;
		mutable unsigned int		instUsed;
		unsigned int				offset;
		unsigned int				len;
	};



	TerrainMesh			m_full;
	TerrainMesh			m_singlePerm;
	TerrainMesh			m_doublePerm;

	mutable bool		m_rescaleInstVBO;
	GLuint				m_InstVBO;
	GLuint				m_VAO;

	GLuint				m_hmapNor;

	HeightMapManager	m_hmapManager;


public:
	Terrain() : m_mesh(), m_grass(AssetManager::getAsset<Texture>(TextureKey("grass.jpg"))),
		m_rock(AssetManager::getAsset<Texture>(TextureKey("rock.jpg"))), m_shader(AssetManager::getAsset<Shader>(ShaderKey("terrain2.frag", "terrain2.vert"))),
		m_rescaleInstVBO(true)
	{
		//***** Setup mesh and instanced rendering buffers *****
		makeMesh(TerrainSettings::CellsPerLeaf, TerrainSettings::MinimumCellLength, m_mesh);

		m_singlePerm.instUsed = m_doublePerm.instUsed = m_full.instUsed = 0;
		m_full.offset = 0;
		m_full.len = m_singlePerm.offset = TerrainSettings::CellsPerLeaf * TerrainSettings::CellsPerLeaf * 6;
		m_singlePerm.len = (TerrainSettings::CellsPerLeaf - 1) * TerrainSettings::CellsPerLeaf * 6 + 9 * TerrainSettings::CellsPerLeaf / 2;
		m_doublePerm.offset = m_singlePerm.offset + m_singlePerm.len;
		m_doublePerm.len = m_mesh.getIndlen() - m_doublePerm.offset;

		//***** Setup heightmap sampler for normal approximation *****
		glGenSamplers(1, &m_hmapNor);
		glSamplerParameteri(m_hmapNor, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(m_hmapNor, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glSamplerParameteri(m_hmapNor, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_hmapNor, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//**** Setup Hardware buffer for instanced rendering and declare vertex format *****
		glGenBuffers(1, &m_InstVBO);

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_mesh.getVertices());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, m_InstVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 4, GL_UNSIGNED_SHORT, sizeof(RenderInstance), NULL);
		glVertexAttribDivisor(1, 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.getIndices());

		glBindVertexArray(0);
	}

	GLuint getTileCache()
	{
		return m_hmapManager.getTileCache();
	}

	HeightMapManager& getHMapManager()
	{
		return m_hmapManager;
	}

	bool HMapLoD(const glm::vec2& min, unsigned int level, glm::lowp_ivec2& offset, const glm::vec3& camPos, ChildLocation loc, HmapNode* node)
	{
		float dist = SqDistanceToPointXZ(min, LevelLen(level) * 0.5f, camPos);
		bool LoDfound = dist >= TerrainSettings::LoDDistances[level] * TerrainSettings::LoDDistances[level];

		return m_hmapManager.LoDCheck(LoDfound, offset, level, node);
	}

	bool LoDCheck(const glm::vec2& min, unsigned int level, glm::lowp_ivec2& offset, const glm::vec3& camPos, ChildLocation loc, HmapNode* node)
	{
		float dist = SqDistanceToPointXZ(min, LevelLen(level) * 0.5f, camPos);
		bool LoDfound = dist >= TerrainSettings::LoDDistances[level] * TerrainSettings::LoDDistances[level];

		m_hmapManager.LoDCheck(LoDfound, offset, level, node);

		if (LoDfound)
		{
			bool Dist1, Dist2;
			unsigned short rot;

			switch (loc)
			{
			case CHILD_SW:
				//W
				Dist1 = SqDistanceToPointXZ(min - glm::vec2(LevelLen(level + 1), LevelLen(level)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//S
				Dist2 = SqDistanceToPointXZ(min + glm::vec2(0.0f, LevelLen(level)), LevelLen(level), camPos)
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
				Dist1 = SqDistanceToPointXZ(min + glm::vec2(LevelLen(level), -LevelLen(level)), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//S
				Dist2 = SqDistanceToPointXZ(min + glm::vec2(-LevelLen(level), LevelLen(level)), LevelLen(level), camPos)
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
				Dist1 = SqDistanceToPointXZ(min - glm::vec2(LevelLen(level + 1), 0.0f), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//N
				Dist2 = SqDistanceToPointXZ(min - glm::vec2(0.0f, LevelLen(level + 1)), LevelLen(level), camPos)
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
				Dist1 = SqDistanceToPointXZ(min + glm::vec2(LevelLen(level), 0.0f), LevelLen(level), camPos)
					>= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1];

				//N
				Dist2 = SqDistanceToPointXZ(min + glm::vec2(-LevelLen(level), -LevelLen(level + 1)), LevelLen(level), camPos)
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
/*
			unsigned int levelPerm = level;

			if (dist >= TerrainSettings::LoDDistances[level + 1] * TerrainSettings::LoDDistances[level + 1])
				levelPerm++;

			dist -= TerrainSettings::LoDDistances[levelPerm] * TerrainSettings::LoDDistances[levelPerm];

			double deltaLoD = TerrainSettings::LoDDistances[levelPerm + 1] * TerrainSettings::LoDDistances[levelPerm + 1] - TerrainSettings::LoDDistances[levelPerm] * TerrainSettings::LoDDistances[levelPerm];
			deltaLoD = (dist - (deltaLoD * (1.0f - TerrainSettings::LoDLerpPercentage))) / (deltaLoD * TerrainSettings::LoDLerpPercentage);

			if (deltaLoD < 0.0)
				deltaLoD = 0.0*/;

			offset -= SHRT_MIN;

			if (rot == 4)
			{
				if (m_full.instances.size() <= m_full.instUsed)
				{
					m_full.instances.push_back(RenderInstance());
					m_rescaleInstVBO = true;
				}

				m_full.instances[m_full.instUsed].offx = offset.x;
				m_full.instances[m_full.instUsed].offz = offset.y;
				m_full.instances[m_full.instUsed].rotscale = level;
				m_full.instances[m_full.instUsed].tex = node->m_tile;
				//full.Instances[full.InstUsed].lodLerp = deltaLoD;

				m_full.instUsed++;
			}
			else if (Dist1 && Dist2)
			{
				if (m_doublePerm.instances.size() <= m_doublePerm.instUsed)
				{
					m_doublePerm.instances.push_back(RenderInstance());
					m_rescaleInstVBO = true;
				}

				m_doublePerm.instances[m_doublePerm.instUsed].offx = offset.x;
				m_doublePerm.instances[m_doublePerm.instUsed].offz = offset.y;
				m_doublePerm.instances[m_doublePerm.instUsed].rotscale = level | (rot << 8);
				m_doublePerm.instances[m_doublePerm.instUsed].tex = node->m_tile;
				//doublePerm.Instances[doublePerm.InstUsed].lodLerp = deltaLoD;

				m_doublePerm.instUsed++;
			}
			else
			{
				if (m_singlePerm.instances.size() <= m_singlePerm.instUsed)
				{
					m_singlePerm.instances.push_back(RenderInstance());
					m_rescaleInstVBO = true;
				}

				m_singlePerm.instances[m_singlePerm.instUsed].offx = offset.x;
				m_singlePerm.instances[m_singlePerm.instUsed].offz = offset.y;
				m_singlePerm.instances[m_singlePerm.instUsed].rotscale = level | (rot << 8);
				m_singlePerm.instances[m_singlePerm.instUsed].tex = node->m_tile;
				//singlePerm.Instances[singlePerm.InstUsed].lodLerp = deltaLoD;

				m_singlePerm.instUsed++;
			}

			return true;
		}

		return false;
	}

	void Render(const glm::mat4& VP, glm::vec3 cam) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_InstVBO);
		if (m_rescaleInstVBO)
		{
			glBufferData(GL_ARRAY_BUFFER, (m_full.instUsed + m_singlePerm.instUsed + m_doublePerm.instUsed) * sizeof(RenderInstance), NULL, GL_DYNAMIC_DRAW);
			m_rescaleInstVBO = false;
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_full.instUsed * sizeof(RenderInstance), m_full.instances.data());
		glBufferSubData(GL_ARRAY_BUFFER, m_full.instUsed * sizeof(RenderInstance), m_singlePerm.instUsed * sizeof(RenderInstance), m_singlePerm.instances.data());
		glBufferSubData(GL_ARRAY_BUFFER, (m_singlePerm.instUsed + m_full.instUsed) * sizeof(RenderInstance), m_doublePerm.instUsed * sizeof(RenderInstance), m_doublePerm.instances.data());

		glBindAttribLocation(m_shader.get().getShader(), 0, "in_Position");
		glBindAttribLocation(m_shader.get().getShader(), 1, "in_InstData");
		glBindAttribLocation(m_shader.get().getShader(), 2, "in_LoDLerp");

		glLinkProgram(m_shader.get().getShader());

		glUseProgram(m_shader.get().getShader());

		GLbyte sp_WVP = glGetUniformLocation(m_shader.get().getShader(), "WVP");
		GLbyte sp_hmap = glGetUniformLocation(m_shader.get().getShader(), "hmap");
		GLbyte sp_hmapNor = glGetUniformLocation(m_shader.get().getShader(), "hmapNor");
		GLbyte sp_gridsize = glGetUniformLocation(m_shader.get().getShader(), "gridSize");
		GLbyte sp_instbuff = glGetUniformLocation(m_shader.get().getShader(), "instBuff");
		GLbyte sp_grass = glGetUniformLocation(m_shader.get().getShader(), "grass");
		GLbyte sp_rock = glGetUniformLocation(m_shader.get().getShader(), "rock");
		GLbyte sp_camPos = glGetUniformLocation(m_shader.get().getShader(), "camPos");
		GLbyte sp_texelSize = glGetUniformLocation(m_shader.get().getShader(), "texelSize");
		GLbyte sp_cellLen = glGetUniformLocation(m_shader.get().getShader(), "cellLen");

		glUniformMatrix4fv(sp_WVP, 1, GL_FALSE, glm::value_ptr(VP));
		glUniform1f(sp_gridsize, TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength);
		glUniform3fv(sp_camPos, 1, glm::value_ptr(cam));
		float txs = 1.0f / static_cast<float>(TerrainSettings::TileCacheSize * TerrainSettings::HMapResWBorder);
		glUniform1f(sp_texelSize, txs);
		glUniform1f(sp_cellLen, TerrainSettings::MinimumCellLength);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_hmapManager.getTileCache());
		glUniform1i(sp_hmap, 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_hmapManager.getTileCache());
		glUniform1i(sp_hmapNor, 1);
		glBindSampler(1, m_hmapNor);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_grass.get().getTexture());
		glUniform1i(sp_grass, 2);

		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, m_rock.get().getTexture());
		glUniform1i(sp_rock, 3);

		glBindVertexArray(m_VAO);

		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, m_full.len      , GL_UNSIGNED_INT, (void*)(m_full.offset       * sizeof(unsigned int)), m_full.instUsed      , 0);
		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, m_singlePerm.len, GL_UNSIGNED_INT, (void*)(m_singlePerm.offset * sizeof(unsigned int)), m_singlePerm.instUsed, m_full.instUsed);
		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, m_doublePerm.len, GL_UNSIGNED_INT, (void*)(m_doublePerm.offset * sizeof(unsigned int)), m_doublePerm.instUsed, m_full.instUsed + m_singlePerm.instUsed);

		glBindVertexArray(0);

		m_singlePerm.instUsed = m_doublePerm.instUsed = m_full.instUsed = 0;
	}

	~Terrain()
	{
		m_mesh.clear();
		glDeleteBuffers(1, &m_InstVBO);
		glDeleteSamplers(1, &m_hmapNor);
		glDeleteVertexArrays(1, &m_VAO);
	}
};