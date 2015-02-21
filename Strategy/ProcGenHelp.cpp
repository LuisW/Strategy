#include "ProcGenHelp.h"
#include "ei/glm/glm.hpp"

std::string genAssetKey(const char* prefix, unsigned int seed)
{
	std::string s = std::string(prefix);

	while (seed > 0)
	{
		s += '0' + seed % 10;
		seed /= 10;
	}

	return s;
}

unsigned int rand(unsigned int seed)
{
	seed = (seed ^ 61u) ^ (seed >> 16u);
	seed *= 9u;
	seed = seed ^ (seed >> 4u);
	seed *= 668265261u;
	return seed ^ (seed >> 15u);
}

unsigned int randuint(unsigned int seed, unsigned int min, unsigned max)
{
	seed = rand(seed);
	seed %= (max - min + 1);
	return seed + min;
}

float randFlt(unsigned int seed, float min, float max)
{
	float r = static_cast <float> (randuint(seed,50000,100000)) / static_cast <float> (50000);
	r -= 1.0f;
	r *= max - min;
	r += min;

	return r;
}

bool randBool(unsigned int seed, float trueProb)
{
	float r = (float)randuint(seed, 0, 10000);
	return r <= trueProb * 10000.0f;
}

float randFltWeighted1(unsigned int seed, float average, float maxDiff) //high probability around average, lower around +/- maxDiff. Values: [ave - maxDiff, ave + maxDiff]
{
	unsigned int r = randuint(seed, 0, 10000);
	float f = glm::pow((float)r / 5000.0f - 1.0f, 3.0f);
	return f * maxDiff + average;
}

float randFltWeighted2(unsigned int seed, float average, float maxDiff) //high probability around average, lower around +/- maxDiff. Values: [ave - maxDiff, ave + maxDiff]
{
	unsigned int r = randuint(seed, 0, 10000);
	float f = glm::pow((float)r / 5000.0f - 1.0f, 3.0f);
	return f * maxDiff + average;
}

float randStatBase(unsigned int seed)
{
	unsigned int r = randuint(seed, 0, 30000);
	float x = (float)r / 10000.0f;
	return glm::pow(x - 1.5874f, 3.0f) + glm::pow(0.55874f * x, 6.0f) + 5;
}

unsigned int ProcMesh::newSurface()
{
	return surfCnt++;
}

unsigned int ProcMesh::setOffset()
{
	offset = vertData.size();
	return offset;
}

unsigned int ProcMesh::AddTriangle(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int surface)
{
	unsigned int baseInd = inds.size();

	inds.push_back(ind0);
	inds.push_back(ind1);
	inds.push_back(ind2);

	glm::vec3 nor = glm::cross(vertData[ind2].pos - vertData[ind1].pos, vertData[ind0].pos - vertData[ind1].pos);

	for (unsigned int n = 0; n < 3; n++)
	{
		std::vector<glm::uvec2>& surfVerts = vertData[inds[baseInd + n]].surfaceVerts;

		bool found = false;

		for (unsigned int sv = 0; sv < surfVerts.size(); sv++)
		{
			if (surfVerts[sv].x == surface)
			{
				found = true;
				inds[baseInd + n] = surfVerts[sv].y;		//Set Index to actual VertexArray index
				verts[surfVerts[sv].y].normal += nor;		//Add nor to normal Sum for averaging
				break;
			}
		}

		if (!found)
		{
			surfVerts.push_back(glm::uvec2(surface, verts.size()));		//Add new surface Vert (surface, VertArray index)
			verts.push_back(Vertex());									//Create new Vertex
			verts.back().pos = vertData[inds[baseInd + n]].pos;			//Set Pos for new Vert
			verts.back().normal = nor;									//Set Nor for new Vert
			inds[baseInd + n] = verts.size() - 1;						//Set Index to actual VertexArray index
		}
	}

	return (inds.size() / 3) - 1;
}

unsigned int ProcMesh::AddQuad(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int ind3, unsigned int surface)
{
	AddTriangle(ind0, ind1, ind2, surface);
	return AddTriangle(ind0, ind2, ind3, surface) - 1;
}

unsigned int ProcMesh::AddTriangleOffset(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int surface)
{
	ind0 += offset;
	ind1 += offset;
	ind2 += offset;

	return AddTriangle(ind0, ind1, ind2, surface);
}

unsigned int ProcMesh::AddQuadOffset(unsigned int ind0, unsigned int ind1, unsigned int ind2, unsigned int ind3, unsigned int surface)
{
	ind0 += offset;
	ind1 += offset;
	ind2 += offset;
	ind3 += offset;

	AddTriangle(ind0, ind1, ind2, surface);
	return AddTriangle(ind0, ind2, ind3, surface) - 1;
}

unsigned int ProcMesh::AddVertex(glm::vec3 _pos)
{
	vertData.push_back(VertData(_pos));
	return verts.size() - 1;
}

unsigned int ProcMesh::AddVertex(float x, float y, float z)
{
	vertData.push_back(VertData(glm::vec3(x,y,z)));
	return verts.size() - 1;
}

Triangle ProcMesh::getTriangle(unsigned int tri)
{
	return Triangle(inds[tri * 3], inds[tri * 3 + 1], inds[tri * 3 + 2]);
}

void ProcMesh::CalcNormals()
{
	for (unsigned int n = 0; n < verts.size(); n++)
	{
		verts[n].normal = glm::normalize(verts[n].normal);
	}
}

Mesh* ProcMesh::makeMesh()
{
	assert(sizeof(Vertex) == sizeof(float)* VertFlts);

	GLuint VBO, IBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*verts.size(), verts.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*inds.size(), inds.data(), GL_STATIC_DRAW);

	unsigned int* indsBuff = new unsigned int[inds.size()];
	char* vertsBuff = new char[verts.size() * sizeof(Vertex)];

	memcpy(indsBuff, inds.data(), sizeof(unsigned int)* inds.size());
	memcpy(vertsBuff, verts.data(), sizeof(Vertex)* verts.size());

	return new Mesh(VBO, IBO, VertFlts*verts.size(), inds.size(), indsBuff, vertsBuff);
}