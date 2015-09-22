#include "Mesh.h"
#include <fstream>

Mesh* LoadTEMMesh_0_9(std::ifstream& file)
{
	bool fmt[5];
	file.read((char*)fmt, sizeof(bool)* 5);

	unsigned int indCnt, vertCnt, boneCnt;
	
	file.read((char*)&indCnt, sizeof(indCnt));
	file.read((char*)&vertCnt, sizeof(vertCnt));
	file.read((char*)&boneCnt, sizeof(boneCnt));

	for (unsigned int n = 0; n < boneCnt; ++n)	//Skip past Bones section. TODO: care about this once bones are implemented
	{
		char padd[512];

		file.read(padd, 130);
		file.read(padd, padd[128]);
	}

	unsigned int* pInds = new unsigned int[indCnt];

	file.read((char*)pInds, indCnt * sizeof(unsigned int));

	VertexAttribFormat format;
	format.addElem(0, GL_FLOAT, 3, SEM_POSITION, 0);

	unsigned int vertSize = 3;
	
	if (fmt[0])				//Normals
	{
		format.addElem(vertSize, GL_FLOAT, 3, SEM_NORMAL, 0);
		vertSize += 3 * sizeof(float);
	}
	
	if (fmt[1])				//Colors
	{
		format.addElem(vertSize, GL_FLOAT, 4, SEM_COLOR, 0);
		vertSize += 4 * sizeof(float);
	}

	if (fmt[2])				//UV1
	{
		format.addElem(vertSize, GL_FLOAT, 2, SEM_TEXCOORD, 0);
		vertSize += 2 * sizeof(float);
	}

	if (fmt[3])				//UV2
	{
		format.addElem(vertSize, GL_FLOAT, 2, SEM_TEXCOORD, 1);
		vertSize += 2 * sizeof(float);
	}

	if (fmt[4])				//Weights
	{
		format.addElem(vertSize, GL_FLOAT, 4, SEM_WEIGHT, 0);
		vertSize += 4 * sizeof(float);
	}

	char* pVerts = new char[vertSize * vertCnt];

	file.read(pVerts, vertSize * vertCnt);

	file.close();

	GLuint VBO, IBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * vertCnt, pVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indCnt, pInds, GL_STATIC_DRAW);

	return new Mesh(VBO, IBO, vertCnt, indCnt, pInds, pVerts, format);
}

Mesh* LoadTEMMesh_0_9_1(std::ifstream& file)
{
	bool fmt[5];
	file.read((char*)fmt, sizeof(bool)* 5);

	unsigned int indCnt, vertCnt, boneCnt;

	file.read((char*)&indCnt, sizeof(indCnt));
	file.read((char*)&vertCnt, sizeof(vertCnt));
	file.read((char*)&boneCnt, sizeof(boneCnt));

	for (unsigned int n = 0; n < boneCnt; ++n)	//Skip past Bones section. TODO: care about this once bones are implemented
	{
		char padd[512];

		file.read(padd, 130);
		file.read(padd, padd[128]);
	}

	float obb[15];

	file.read((char*)obb, sizeof(float)* 15);

	OBB boundingBox = OBB(glm::vec4(obb[3], obb[4], obb[5], obb[6]),
						  glm::vec4(obb[7], obb[8], obb[9], obb[10]), 
						  glm::vec4(obb[11], obb[12], obb[13], obb[14]),
						  glm::vec3(obb[0], obb[1], obb[2]));

	unsigned int* pInds = new unsigned int[indCnt];

	file.read((char*)pInds, indCnt * sizeof(unsigned int));

	VertexAttribFormat format;
	format.addElem(0, GL_FLOAT, 3, SEM_POSITION, 0);

	unsigned int vertSize = 3 * sizeof(float);

	if (fmt[0])				//Normals
	{
		format.addElem(vertSize, GL_FLOAT, 3, SEM_NORMAL, 0);
		vertSize += 3 * sizeof(float);
	}

	if (fmt[1])				//Colors
	{
		format.addElem(vertSize, GL_FLOAT, 4, SEM_COLOR, 0);
		vertSize += 4 * sizeof(float);
	}

	if (fmt[2])				//UV1
	{
		format.addElem(vertSize, GL_FLOAT, 2, SEM_TEXCOORD, 0);
		vertSize += 2 * sizeof(float);
	}

	if (fmt[3])				//UV2
	{
		format.addElem(vertSize, GL_FLOAT, 2, SEM_TEXCOORD, 1);
		vertSize += 2 * sizeof(float);
	}

	if (fmt[4])				//Weights
	{
		format.addElem(vertSize, GL_FLOAT, 4, SEM_WEIGHT, 0);
		vertSize += 4 * sizeof(float);
	}

	char* pVerts = new char[vertSize * vertCnt];

	file.read(pVerts, vertSize * vertCnt);

	file.close();

	GLuint VBO, IBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * vertCnt, pVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indCnt, pInds, GL_STATIC_DRAW);

	return new Mesh(VBO, IBO, vertCnt, indCnt, pInds, pVerts, boundingBox, format);
}

Mesh* MeshAssetManager::Load(MeshKey key)
{
	try
	{
		std::ifstream file(key.m_sName, std::ios::binary | std::ios::in);

		if (!file)
			throw AssetLoadException<Mesh>(key, "Couldn't open file!");

		unsigned int ver = 0;
		file.read((char*)&ver, sizeof(ver));

		switch (ver)
		{
		case 900:
			return LoadTEMMesh_0_9(file);
		case 910:
			return LoadTEMMesh_0_9_1(file);
		default:
			throw std::exception("Unknown format");
		}

	}
	catch (std::exception e)
	{
		return NULL;
	}
}

void MeshAssetManager::Unload(AssetIterator it)
{
	it->second.m_pData->clear();
}