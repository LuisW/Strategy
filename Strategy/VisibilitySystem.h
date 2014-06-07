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
	QTCullGlobalPass(Terrain& _terrain, Frustum& _frustrum, const glm::vec3& _camPos) 
		: terrain(_terrain), frustrum(_frustrum), camPos(_camPos)
	{}

	Terrain& terrain;
	Frustum& frustrum;
	const glm::vec3& camPos;
};

struct QTCullLocalPass
{
	QTCullLocalPass(unsigned char _level, char _parentIn, glm::vec3 _h)
		: level(_level), parentIn(_parentIn), h(_h)
	{}

	unsigned int index;
	unsigned char level;
	char parentIn;
	ChildLocation loc;
	glm::lowp_uvec2 offset;
	glm::vec3 h;
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
		glm::vec2 min = glm::vec2(local.offset) * (TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength);

  		IntersectionType it = global.frustrum.IntersectAABB(glm::vec3(min.x, 0.0f, min.y) + local.h , local.h ,local.parentIn);

		if (it == IT_Intersect)
		{
			bool LoDfound = global.terrain.LoDCheck(min, local.level, local.offset, global.camPos, local.loc);

			if (local.level != 0 && !LoDfound)
			{
				QTCullLocalPass newLocal = QTCullLocalPass(local.level - 1, local.parentIn, glm::vec3((TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength) * (1 << (local.level - 1))));
				newLocal.h.y = local.h.y;

				for (unsigned int n = 0; n < 4; n++)
				{
					newLocal.index = local.index + n * levelFactors[newLocal.level] + 1;
					newLocal.loc = ChildLocation(n);

					switch (newLocal.loc)
					{
					case CHILD_NW:
						newLocal.offset = local.offset;
						break;
					case CHILD_NE:
						newLocal.offset = glm::lowp_uvec2(local.offset.x + (1 << newLocal.level), local.offset.y);
						break;
					case CHILD_SW:
						newLocal.offset = glm::lowp_uvec2(local.offset.x, local.offset.y + (1 << newLocal.level));
						break;
					case CHILD_SE:
						newLocal.offset = glm::lowp_uvec2(local.offset.x + (1 << newLocal.level), local.offset.y + (1 << newLocal.level));
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
	}

	static void SetAllChildrenIn(QTCullGlobalPass& global, QTCullLocalPass& local)
	{
		bool LoDfound = global.terrain.LoDCheck(glm::vec2(local.offset) * (TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength), local.level, local.offset, global.camPos, local.loc);

		if (local.level != 0 && !LoDfound)
		{
			QTCullLocalPass newLocal = QTCullLocalPass(local.level - 1, local.parentIn, glm::vec3());

			for (unsigned int n = 0; n < 4; n++)
			{
				newLocal.index = local.index + n * levelFactors[newLocal.level] + 1;
				newLocal.loc = ChildLocation(n);

				switch (newLocal.loc)
				{
				case CHILD_NW:
					newLocal.offset = local.offset;
					break;
				case CHILD_NE:
					newLocal.offset = glm::lowp_uvec2(local.offset.x + (1 << newLocal.level), local.offset.y);
					break;
				case CHILD_SW:
					newLocal.offset = glm::lowp_uvec2(local.offset.x, local.offset.y + (1 << newLocal.level));
					break;
				case CHILD_SE:
					newLocal.offset = glm::lowp_uvec2(local.offset.x + (1 << newLocal.level), local.offset.y + (1 << newLocal.level));
					break;
				}

				SetAllChildrenIn(global, newLocal);
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
	unsigned int level;
	Terrain& terrain;
	glm::lowp_uvec2 globalOffset;

public:
	TopQTNode(unsigned int _level, Terrain& _terrain) : terrain(_terrain), level(_level)
	{
		
	}

	void Feed(const glm::lowp_uvec2& _globalOffset)
	{
		globalOffset = _globalOffset;
	}

	void Cull(const glm::vec3& camPos, Frustum& frustrum)
	{
		QTCullGlobalPass global(terrain, frustrum, camPos);
		QTCullLocalPass local = QTCullLocalPass(level, 0, glm::vec3((TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength) * (1 << (level - 1))));
		local.h.y = 1000.0f;
		local.index = 0;
		local.loc = ChildLocation(0);
		local.offset = globalOffset;

		QTNode::Cull(global, local);
	}

	~TopQTNode()
	{
	}
};


#define NTopNodes 9

class VisibilitySystem //: public System
{
private:
	struct TopNode
	{
		TopQTNode* node;
		bool used;
	};

	EntityManager& entities;
	Terrain& terrain;

	TopNode topNodes[NTopNodes];
	unsigned int treeDepth;

public:
	VisibilitySystem(EntityManager& _entities, unsigned int _treeDepth, Terrain& _terrain) : entities(_entities), treeDepth(_treeDepth), terrain(_terrain)
	{
		for (unsigned int n = 0; n < NTopNodes; n++)
		{
			unsigned short topSize = 1 << treeDepth;
			topNodes[n].node = new TopQTNode(_treeDepth, terrain);
			topNodes[n].node->Feed(glm::lowp_uvec2(topSize * (n/3), topSize * (n%3)));
			topNodes[n].used = true;
		}
	}

	void Tick(const Camera& cam)
	{
		for (unsigned int n = 0; n < NTopNodes; n++)
		{
			if (topNodes[n].used)
			{
				topNodes[n].node->Cull(cam.getPos(), cam.getFrustum());
			}
		}
	}

	~VisibilitySystem()
	{
		for (unsigned int n = 0; n < NTopNodes; n++)
		{
			delete topNodes[n].node;
		}
	}

};