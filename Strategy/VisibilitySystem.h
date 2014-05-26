#pragma once

#include "EntityManager.h"	
#include "Camera.h"
#include "Terrain.h"

class QTNode;

struct QTCullPassStruct
{
	QTCullPassStruct(QTNode* _nodes, Terrain& _terrain, Frustrum& _frustrum, const glm::vec3& _camPos) 
		: nodes(_nodes), terrain(_terrain), frustrum(_frustrum), camPos(_camPos)
	{}

	QTNode* nodes;
	Terrain& terrain;
	Frustrum& frustrum;
	const glm::vec3& camPos;
};

class TopQTNode;

class QTNode
{
private:
	friend TopQTNode;

	glm::lowp_uvec2 offset;
	AABoundingBox boundingBox;
	unsigned char failPlane;

public:
	QTNode() : boundingBox(glm::vec3(0.0f), glm::vec3(0.0f)), failPlane(FP_Top)
	{

	}

	QTNode(const AABoundingBox& _boundingBox, unsigned int level, glm::lowp_uvec2 _offset) : boundingBox(_boundingBox),
		failPlane(FP_Top), offset(_offset)
	{

	}

	void Cull(QTCullPassStruct& data, unsigned int index, unsigned char level, char parentIn)
	{
		IntersectionType it = data.frustrum.IntersectAABB(boundingBox, parentIn, failPlane);

		if (it == IT_Intersect)
		{
			bool LoDfound = data.terrain.LoDCheck(boundingBox, level, offset, data.camPos);

			if (level != 0 && !LoDfound)
			{
				--level;

				unsigned int levelFac = ((1 << (2 * level + 2)) - 1) / 3;
				unsigned int currInd = index;

				for (unsigned int n = 0; n < 4; n++)
				{
					index = currInd + n * levelFac + 1;
					data.nodes[index].Cull(data, index, level, parentIn);
				}
			}
		}
		else if (it == IT_Inside)
		{
			SetAllChildrenIn(data, index, level);
		}
	}

	void SetAllChildrenIn(QTCullPassStruct& data, unsigned int index, unsigned char level)
	{
		bool LoDfound = data.terrain.LoDCheck(boundingBox, level, offset, data.camPos);

		if (level != 0 && !LoDfound)
		{
			--level;

			unsigned int levelFac = ((1 << (2 * level + 2)) - 1) / 3;
			unsigned int currInd = index;

			for (unsigned int n = 0; n < 4; n++)
			{
				index = currInd + n * levelFac + 1;
				data.nodes[index].SetAllChildrenIn(data, index, level);
			}
		}
	}

	void Feed(QTNode* nodes, unsigned char level, unsigned int index)
	{
		if (level > 0)
		{
			glm::vec3 min = boundingBox.getMin();
			glm::vec3 max = boundingBox.getMax();
			glm::vec3 center = boundingBox.getCenter();

			--level;

			unsigned int levelFac = ((1 << (2 * (level) + 2)) - 1) / 3;

			unsigned int c0 = index + 1;
			unsigned int c1 = levelFac + index + 1;
			unsigned int c2 = levelFac * 2 + index + 1;
			unsigned int c3 = levelFac * 3 + index + 1;

			unsigned short childlen = 1 << level;
			nodes[c0].boundingBox = AABoundingBox(glm::vec3(min.x, 0, min.z), glm::vec3(center.x, 255, center.z));
			nodes[c0].offset = offset;
			nodes[c0].Feed(nodes, level, c0);

			nodes[c1].boundingBox = AABoundingBox(glm::vec3(center.x, 0, min.z), glm::vec3(max.x, 255, center.z));
			nodes[c1].offset = glm::lowp_uvec2(offset.x + childlen, offset.y);
			nodes[c1].Feed(nodes, level, c1);

			nodes[c2].boundingBox = AABoundingBox(glm::vec3(min.x, 0, center.z), glm::vec3(center.x, 255, max.z));
			nodes[c2].offset = glm::lowp_uvec2(offset.x, offset.y + childlen);
			nodes[c2].Feed(nodes, level, c2);

			nodes[c3].boundingBox = AABoundingBox(glm::vec3(center.x, 0, center.z), glm::vec3(max.x, 255, max.z));
			nodes[c3].offset = offset + childlen;
			nodes[c3].Feed(nodes, level, c3);
		}
	}

	~QTNode()
	{
	}
};


class TopQTNode
{
private:
	unsigned int level;
	unsigned int nNodes;
	QTNode* QTNodes;
	Terrain& terrain;

public:
	TopQTNode(unsigned int _level, Terrain& _terrain) : terrain(_terrain), level(_level), nNodes(((1 << (2 * level + 2)) - 1) / 3), QTNodes(new QTNode[nNodes])
	{
		
	}

	void Feed(AABoundingBox& boundingBox, const glm::lowp_uvec2& offset)
	{
		QTNodes[0].boundingBox = boundingBox;
		QTNodes[0].offset = offset;
		QTNodes[0].Feed(QTNodes, level, 0);
	}

	void Cull(const glm::vec3& camPos, Frustrum& frustrum)
	{
		QTCullPassStruct data(QTNodes, terrain, frustrum, camPos);

		QTNodes[0].Cull(data, 0, level, 0);
	}

	~TopQTNode()
	{
		delete[] QTNodes;
	}
};


#define NTopNodes 1

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
			float topSize = TerrainSettings::CellsPerLeaf * TerrainSettings::MinimumCellLength * (1 << _treeDepth);
			topNodes[n].node = new TopQTNode(_treeDepth, terrain);
			topNodes[n].node->Feed(AABoundingBox(glm::vec3(0.0f), glm::vec3(topSize, 2000.0f, topSize)), glm::lowp_uvec2(0, 0));
			topNodes[n].used = true;
		}
	}

	void Tick(const Camera& cam)
	{
		for (unsigned int n = 0; n < NTopNodes; n++)
		{
			if (topNodes[n].used)
			{
				topNodes[n].node->Cull(cam.getPos(), cam.getFrustrum());
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