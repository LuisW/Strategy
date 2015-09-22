#pragma once
#include "DataStructs.h"
#include <fstream>

enum ChildLocation { CHILD_NW, CHILD_NE, CHILD_SW, CHILD_SE };

using namespace TerrainSettings;

inline float SqDistanceToPointXZ(const glm::vec2& min, float lenOver2, const glm::vec3& p)
{
	float dx = glm::max(glm::abs(p.x - min.x - lenOver2) - lenOver2, 0.0f);
	float dz = glm::max(glm::abs(p.z - min.y - lenOver2) - lenOver2, 0.0f);

	return dx*dx + dz*dz;
}

inline float LevelLen(unsigned int level)
{
	return TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength * static_cast<float>(1 << level);
}

class HmapNode
{
private:

public:
	short		m_tile;
	HmapNode*	m_pChildren;

	void CheckLoD(glm::vec3& camPos, unsigned int level, glm::ivec2& off)
	{
	}

	~HmapNode()
	{
		delete[] m_pChildren;
	}
};

class HeightMapManager
{
private:
	static const unsigned int CACHE_LEN = TileCacheSize * TileCacheSize;
	static const unsigned int MIP_MAP_RES = HeightMapRes / 2 + 1;
	static const unsigned int MAP_OFF = 1 << (TerrainSettings::LoDs - 1);
	static const unsigned int FULL_MAP_RES = (TerrainSettings::HeightMapRes - 1) * TerrainSettings::HeightMapCnt + 1;

	struct PhysicalTile
	{
		bool active;
	};

	struct LoadToPrecache
	{

	};

	struct LoadToHW
	{

	};

	HmapNode		m_topNode;
	GLuint			m_tileCache;
	PhysicalTile	m_tiles[CACHE_LEN];
	std::ifstream	m_hmapFile;

	void LoadTile(const glm::lowp_ivec2&  offset, HmapNode* node, unsigned int level) 
	{
		unsigned int x = (offset.x + MAP_OFF) * TerrainSettings::CellsPerLeaf;
		unsigned int y = (offset.y + MAP_OFF) * TerrainSettings::CellsPerLeaf;

		unsigned int rows = (1 << level << 5);
		unsigned int step = rows >> 7;
		rows++;
		Uint16* readBuff = new Uint16[rows];
		Uint16* tileBuff = new Uint16[TerrainSettings::HMapResWBorder * (TerrainSettings::HMapResWBorder + 1)];

		for (unsigned int n = 0; n < TerrainSettings::HeightMapRes; n++)
		{
			m_hmapFile.seekg((y * FULL_MAP_RES + x) * sizeof(Uint16));
			m_hmapFile.read((char*)readBuff, rows * sizeof(Uint16));

			y += step;
			for (unsigned int i = 0; i < TerrainSettings::HeightMapRes; i++)
			{
				tileBuff[(n + 2) * (HMapResWBorder + 1) + i + 2] = readBuff[i * step];

				if (i == 0)
				{
					tileBuff[(n + 2) * (HMapResWBorder + 1)] = readBuff[0];
					tileBuff[(n + 2) * (HMapResWBorder + 1) + 1] = readBuff[0];
				}
				else if (i == TerrainSettings::HeightMapRes - 1)
				{
					tileBuff[(n + 2) * (HMapResWBorder + 1) + i + 4] = readBuff[i * step];
					tileBuff[(n + 2) * (HMapResWBorder + 1) + i + 3] = readBuff[i * step];
				}
			}

			memcpy(tileBuff, tileBuff + 2 * (HMapResWBorder + 1), sizeof(Uint16)* (HMapResWBorder + 1));
			memcpy(tileBuff + (HMapResWBorder + 1), tileBuff + 2 * (HMapResWBorder + 1), sizeof(Uint16)* (HMapResWBorder + 1));

			memcpy(tileBuff + (HMapResWBorder + 1) * (HMapResWBorder - 1), tileBuff + (HMapResWBorder + 1) * (HMapResWBorder - 3), sizeof(Uint16)* (HMapResWBorder + 1));
			memcpy(tileBuff + (HMapResWBorder + 1) * (HMapResWBorder - 2), tileBuff + (HMapResWBorder + 1) * (HMapResWBorder - 3), sizeof(Uint16)* (HMapResWBorder + 1));

		}

		node->m_tile = AddTile(tileBuff, NULL);
		delete[] tileBuff;
		delete[] readBuff;
	}

	void InitRecursive(const glm::vec3& camPos, glm::lowp_ivec2& offset, unsigned int level, HmapNode* node)
	{
		glm::vec2 min = glm::vec2(offset) * (TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength);

		float dist = INFINITY;
		if (level > 2)
		{
			dist = SqDistanceToPointXZ(min, LevelLen(level) * 0.5f, camPos);
		}

		if (dist >= TerrainSettings::LoDDistances[level] * TerrainSettings::LoDDistances[level])
		{
			LoadTile(offset, node, level);
		}
		else
		{
			node->m_pChildren = new HmapNode[4];
			for (unsigned int n = 0; n < 4; n++)
			{
				node->m_pChildren[n].m_tile = -1;
				node->m_pChildren[n].m_pChildren = NULL;
				unsigned int newLevel = level - 1;

				glm::lowp_ivec2 newOff;

				switch (n)
				{
				case CHILD_NW:
					newOff = offset;
					break;
				case CHILD_NE:
					newOff = glm::lowp_ivec2(offset.x + (1 << newLevel), offset.y);
					break;
				case CHILD_SW:
					newOff = glm::lowp_ivec2(offset.x, offset.y + (1 << newLevel));
					break;
				case CHILD_SE:
					newOff = glm::lowp_ivec2(offset.x + (1 << newLevel), offset.y + (1 << newLevel));
					break;
				}

				InitRecursive(camPos, newOff, newLevel, &(node->m_pChildren[n]));
			}
		}
	}

public:
	HeightMapManager()
	{
		//***** Setup Heightmap Tilechache *****
		glGenTextures(1, &m_tileCache);
		glBindTexture(GL_TEXTURE_2D, m_tileCache);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16, TerrainSettings::HMapResWBorder * TerrainSettings::TileCacheSize, TerrainSettings::HMapResWBorder * TerrainSettings::TileCacheSize);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//***** Open File for Streaming *****
		m_hmapFile.open("Test.hmap", std::ios::in | std::ios::binary);

		memset(m_tiles, 0, sizeof(PhysicalTile)* CACHE_LEN);
	}

	HmapNode& getTopNode()
	{
		return m_topNode;
	}

	GLuint getTileCache() const
	{
		return m_tileCache;
	}

	void Init(const glm::vec3& camPos)
	{
		glBindTexture(GL_TEXTURE_2D, m_tileCache);
		InitRecursive(camPos, -glm::lowp_ivec2(MAP_OFF), TerrainSettings::LoDs, &m_topNode);
	}

	bool LoDCheck(bool LoDFound, const glm::lowp_ivec2 offset, unsigned int level, HmapNode* node)
	{
		if (level == TerrainSettings::LoDs)
		{
			glBindTexture(GL_TEXTURE_2D, m_tileCache);
		}

		if (level < 2)
		{
			return true;
		}

		if (LoDFound || level == 2)
		{
			if (node->m_tile >= 0)
			{
				return true;
			}
			
			if (node->m_pChildren != NULL)	//Moved Up
			{
				for (unsigned int n = 0; n < 4; n++)
				{
					RemoveTile(node->m_pChildren[n].m_tile);
				}
				delete[] node->m_pChildren;
				node->m_pChildren = NULL;
				LoadTile(offset, node, level);
			}
			else         //Moved Down
			{
				LoadTile(offset, node, level);
			}
			return true;
		}
		else
		{
			if (node->m_pChildren == NULL)
			{
				node->m_pChildren = new HmapNode[4];
				for (unsigned int n = 0; n < 4; n++)
				{
					node->m_pChildren[n].m_tile = -1;
					node->m_pChildren[n].m_pChildren = NULL;
				}
				
				RemoveTile(node->m_tile);
				node->m_tile = -1;
			}

			return false;
		}
	}

	unsigned int AddTile(Uint16* LoD0, Uint16* LoD1)
	{
		for (int n = 0; n < CACHE_LEN; n++)
		{
			if (!m_tiles[n].active)
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, (n / TileCacheSize) * HMapResWBorder, (n % TileCacheSize) * HMapResWBorder, HMapResWBorder, HMapResWBorder, GL_RED, GL_UNSIGNED_SHORT, LoD0);
				//glTexSubImage2D(GL_TEXTURE_2D, 1, (n / TileCacheSize) * MIP_MAP_RES, (n % TileCacheSize) * MIP_MAP_RES, MIP_MAP_RES, MIP_MAP_RES, GL_RED, GL_UNSIGNED_SHORT, LoD1);
				m_tiles[n].active = true;
				return n;
			}
		}
	}

	inline void RemoveTile(unsigned int tile)
	{
		m_tiles[tile].active = false;
	}

	~HeightMapManager()
	{
		glDeleteTextures(1, &m_tileCache);
		m_hmapFile.close();
	}
};