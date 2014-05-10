#pragma once

#include "System.h"	
#include "Frustrum.h"

enum QTChildPosition {QT_TopRight, QT_BottomRight, QT_TopLeft, QT_BottomLeft};

class QTNode
{
private:
	QTNode** children;
	QTNode* parent;

	unsigned int level;
	QTChildPosition position;

	AABoundingBox boundingBox;
	FrustrumPlane failPlane;

	//TerrainChunk* Chunk;

public:

	QTNode(const AABoundingBox& _boundingBox, unsigned int _level, QTNode* _parent, QTChildPosition _position) : boundingBox(_boundingBox), level(_level), parent(_parent), position(_position), failPlane(FP_Top)
	{
		if (level > 0)
		{
			children = new QTNode*[4];

			glm::vec3 min = boundingBox.getMin();
			glm::vec3 max = boundingBox.getMax();
			glm::vec3 center = boundingBox.getCenter();

			children[QT_BottomLeft] = new QTNode(AABoundingBox(glm::vec3(min.x, 0, min.z), glm::vec3(center.x, 255, center.z)), level - 1, this, QT_BottomLeft);
			children[QT_BottomRight] = new QTNode(AABoundingBox(glm::vec3(center.x, 0, min.z), glm::vec3(max.x, 255, center.z)), level - 1, this, QT_BottomRight);
			children[QT_TopLeft] = new QTNode(AABoundingBox(glm::vec3(min.x, 0, center.z), glm::vec3(center.x, 255, max.z)), level - 1, this, QT_TopLeft);
			children[QT_TopRight] = new QTNode(AABoundingBox(glm::vec3(center.x, 0, center.z), glm::vec3(max.x, 255, max.z)), level - 1, this, QT_TopRight);
		}
	}


	void Cull(char parentIn, Frustrum& frustrum)
	{
		IntersectionType it = frustrum.IntersectAABB(boundingBox, parentIn, failPlane);

		if (it == IT_Intersect)
		{
			if (children != NULL)
			{
				for (unsigned int n = 0; n < 4; n++)
				{
					children[0]->Cull(parentIn, frustrum);
				}
			}
		}
		else if (it == IT_Inside)
		{
			SetAllChildrenIn();
		}
		else
		{
			SetAllChildrenOut();
		}
	}

	void SetAllChildrenIn()
	{
		if (children != NULL)
		{
			for (unsigned int n = 0; n < 4; n++)
			{
				children[n]->SetAllChildrenIn();
			}
		}
	}

	void SetAllChildrenOut()
	{
		if (children != NULL)
		{
			for (unsigned int n = 0; n < 4; n++)
			{
				children[n]->SetAllChildrenOut();
			}
		}
	}
};

class VisibilitySystem : public System
{
private:
	EntityManager& entities;

	QTNode** topNodes;
	unsigned int treeDepth;

public:
	VisibilitySystem(EntityManager& _entities, unsigned int treeDepth) : entities(_entities)
	{}
};