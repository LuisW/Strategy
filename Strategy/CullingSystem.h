#pragma once

#include "EntityManager.h"	
#include "Camera.h"
#include "Terrain.h"

#define calcLevelFac(level) ((1 << (2 * level + 2)) - 1) / 3

const unsigned int levelFactors[] = { calcLevelFac(0), calcLevelFac(1), calcLevelFac(2), calcLevelFac(3), calcLevelFac(4), calcLevelFac(5), calcLevelFac(6),
calcLevelFac(7), calcLevelFac(8), calcLevelFac(9), calcLevelFac(10), calcLevelFac(11), calcLevelFac(12) };

class QTNode;

struct QTCullGlobalPass
{
	QTCullGlobalPass(Terrain& terrain, Frustum& frustrum, const glm::vec3& camPos) 
		: m_rTerrain(terrain), m_rFrustrum(frustrum), m_rCamPos(camPos)
	{}

	Terrain&			m_rTerrain;
	Frustum&			m_rFrustrum;
	const glm::vec3&	m_rCamPos;
};

struct QTCullLocalPass
{
	QTCullLocalPass(unsigned char level, char parentIn, glm::vec3 dimensions)
		: m_level(level), m_parentIn(parentIn), m_dimensions(dimensions)
	{}

	unsigned char	m_level;
	char			m_parentIn;
	ChildLocation	m_loc;
	glm::lowp_ivec2 m_offset;
	glm::vec3		m_dimensions;
	HmapNode*		m_pNode;
};

class TopQTNode;

class QTNode
{
private:
	friend TopQTNode;

public:
	QTNode()
	{

	}

	static void Cull(QTCullGlobalPass& global, QTCullLocalPass& local)
	{
		glm::vec2 min = glm::vec2(local.m_offset) * (TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength);

  		IntersectionType it = global.m_rFrustrum.IntersectAABB(glm::vec3(min.x, 0.0f, min.y) + local.m_dimensions , local.m_dimensions ,local.m_parentIn);

		if (local.m_level == 0 && it != IT_Intersect)
		{
			it = it;
		}

		if (it == IT_Intersect)
		{
			bool LoDfound = global.m_rTerrain.LoDCheck(min, local.m_level, local.m_offset, global.m_rCamPos, local.m_loc, local.m_pNode);
			
			if (local.m_level != 0 && !LoDfound)
			{
				QTCullLocalPass newLocal = QTCullLocalPass(local.m_level - 1, local.m_parentIn, glm::vec3((TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength) * (1 << (local.m_level - 1))));
				newLocal.m_dimensions.y = local.m_dimensions.y;

				for (unsigned int n = 0; n < 4; n++)
				{
					newLocal.m_loc = ChildLocation(n);
					if (local.m_level > 2)
					{
						newLocal.m_pNode = &(local.m_pNode->m_pChildren[newLocal.m_loc]);
					}
					else
					{
						newLocal.m_pNode = local.m_pNode;
					}

					switch (newLocal.m_loc)
					{
					case CHILD_NW:
						newLocal.m_offset = local.m_offset;
						break;
					case CHILD_NE:
						newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x + (1 << newLocal.m_level), local.m_offset.y);
						break;
					case CHILD_SW:
						newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x, local.m_offset.y + (1 << newLocal.m_level));
						break;
					case CHILD_SE:
						newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x + (1 << newLocal.m_level), local.m_offset.y + (1 << newLocal.m_level));
						break;
					}

					Cull(global, newLocal);
				}
			}
		}
		else if (it == IT_Inside)
		{
			SetAllChildrenIn(global, local);
		}
		else
		{
			FindHMapLoD(global, local);
		}
	}

	static void SetAllChildrenIn(QTCullGlobalPass& global, QTCullLocalPass& local)
	{
		glm::vec2 min = glm::vec2(local.m_offset) * (TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength);
		bool LoDfound = global.m_rTerrain.LoDCheck(min, local.m_level, local.m_offset, global.m_rCamPos, local.m_loc, local.m_pNode);
			
		if (local.m_level != 0 && !LoDfound)
		{
			QTCullLocalPass newLocal = QTCullLocalPass(local.m_level - 1, local.m_parentIn, glm::vec3());

			for (unsigned int n = 0; n < 4; n++)
			{
				newLocal.m_loc = ChildLocation(n);
				if (local.m_level > 2)
				{
					newLocal.m_pNode = &(local.m_pNode->m_pChildren[newLocal.m_loc]);
				}
				else
				{
					newLocal.m_pNode = local.m_pNode;
				}

				switch (newLocal.m_loc)
				{
				case CHILD_NW:
					newLocal.m_offset = local.m_offset;
					break;
				case CHILD_NE:
					newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x + (1 << newLocal.m_level), local.m_offset.y);
					break;
				case CHILD_SW:
					newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x, local.m_offset.y + (1 << newLocal.m_level));
					break;
				case CHILD_SE:
					newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x + (1 << newLocal.m_level), local.m_offset.y + (1 << newLocal.m_level));
					break;
				}

				SetAllChildrenIn(global, newLocal);
			}
		}
	}

	static void FindHMapLoD(QTCullGlobalPass& global, QTCullLocalPass& local)
	{
		glm::vec2 min = glm::vec2(local.m_offset) * (TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength);
		bool LoDfound = global.m_rTerrain.HMapLoD(min, local.m_level, local.m_offset, global.m_rCamPos, local.m_loc, local.m_pNode);

		if (local.m_level > 1 && !LoDfound)
		{
			QTCullLocalPass newLocal = QTCullLocalPass(local.m_level - 1, local.m_parentIn, glm::vec3());

			for (unsigned int n = 0; n < 4; n++)
			{
				newLocal.m_loc = ChildLocation(n);
				newLocal.m_pNode = &(local.m_pNode->m_pChildren[newLocal.m_loc]);

				switch (newLocal.m_loc)
				{
				case CHILD_NW:
					newLocal.m_offset = local.m_offset;
					break;
				case CHILD_NE:
					newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x + (1 << newLocal.m_level), local.m_offset.y);
					break;
				case CHILD_SW:
					newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x, local.m_offset.y + (1 << newLocal.m_level));
					break;
				case CHILD_SE:
					newLocal.m_offset = glm::lowp_ivec2(local.m_offset.x + (1 << newLocal.m_level), local.m_offset.y + (1 << newLocal.m_level));
					break;
				}

				FindHMapLoD(global, newLocal);
			}
		}
	}

	void Feed(QTNode* nodes, unsigned char level, unsigned int index)
	{

	}

	~QTNode()
	{
	}
};


class TopQTNode
{
private:
	unsigned int m_level;
	Terrain& m_rTerrain;
	glm::lowp_ivec2 m_globalOffset;

public:
	TopQTNode(unsigned int _level, Terrain& _terrain) : m_rTerrain(_terrain), m_level(_level)
	{
		
	}

	void Feed(const glm::lowp_ivec2& _globalOffset)
	{
		m_globalOffset = _globalOffset;
	}

	void Cull(const glm::vec3& camPos, Frustum& frustrum)
	{
		QTCullGlobalPass global(m_rTerrain, frustrum, camPos);
		QTCullLocalPass local = QTCullLocalPass(m_level, 0, glm::vec3((TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength) * (1 << (m_level - 1))));
		local.m_dimensions.y = 1000.0f;
		local.m_loc = ChildLocation(0);
		local.m_offset = m_globalOffset;
		local.m_pNode = &(m_rTerrain.getHMapManager().getTopNode());

		QTNode::Cull(global, local);
	}

	~TopQTNode()
	{
	}
};

class CullingSystem : public System
{
private:
	EntityManager& m_rEntities;
	Terrain& m_rTerrain;

	TopQTNode* m_pTopNode;
	unsigned int m_treeDepth;

public:
	CullingSystem(EntityManager& _entities, unsigned int _treeDepth, Terrain& _terrain) : m_rEntities(_entities), m_treeDepth(_treeDepth), m_rTerrain(_terrain)
	{
		unsigned short topSize = 1 << (m_treeDepth-1);
		m_pTopNode = new TopQTNode(_treeDepth, m_rTerrain);
		m_pTopNode->Feed(glm::lowp_ivec2(-topSize , -topSize));
	}

	void Tick(const Camera& cam)
	{
		m_pTopNode->Cull(cam.getPos(), cam.getFrustum());
	}

	void onEntityChanged(EntityID id, ComponentType type, bool added)
	{

	}

	void onEntityRemoved(EntityID id)
	{

	}

	~CullingSystem()
	{
		delete m_pTopNode;
	}
};