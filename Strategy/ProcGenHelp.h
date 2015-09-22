#pragma once

#include<string>
#include<vector>
#include"Mesh.h"

std::string genAssetKey(const char* prefix, unsigned int seed);

unsigned int rand(unsigned int seed);
unsigned int randuint(unsigned int seed, unsigned int min, unsigned max);
float randFlt(unsigned int seed, float min, float max);
bool randBool(unsigned int seed, float trueProb);
float randFltWeighted1(unsigned int seed, float average, float maxDiff);
glm::vec2 randPoint(const glm::vec2& center, float rad, unsigned int seed);
float randStatBase(unsigned int seed);

struct Triangle
{
	unsigned int p1, p2, p3;
	Triangle(unsigned int _p1, unsigned int _p2, unsigned int _p3) : p1(_p1), p2(_p2), p3(_p3)
	{}
};

class ProcMesh
{
private:
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
#define VertFlts 6
	};

	struct VertZone
	{
		bool used;
		glm::vec3 normal;
		unsigned int vert[2];
	};

	struct VertData
	{
		std::vector<glm::uvec2> surfaceVerts;
		glm::vec3 pos;

		VertData(glm::vec3& _pos) : pos(_pos)
		{}
	};

	std::vector<unsigned int> inds;
	std::vector<VertData> vertData;
	std::vector<Vertex> verts;

	unsigned int offset;
	unsigned int surfCnt;

public:
	ProcMesh() : surfCnt(0)
	{}

	unsigned int newSurface();
	unsigned int setOffset();
	unsigned int AddTriangle(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int surface);
	unsigned int AddQuad(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int ind3, unsigned int surface);
	unsigned int AddTriangleOffset(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int surface);
	unsigned int AddQuadOffset(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int ind3, unsigned int surface);
	unsigned int AddVertex(glm::vec3 _pos);
	unsigned int AddVertex(float x, float y, float z);
	Triangle getTriangle(unsigned int tri);
	void CalcNormals();
	Mesh* makeMesh();
};