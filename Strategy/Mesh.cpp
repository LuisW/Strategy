#include "Mesh.h"
#include <fstream>

Mesh* LoadTEMMesh_0_9(std::ifstream& file)
{
	bool fmt[5];
	file.read((char*)fmt, sizeof(bool)* 5);

	unsigned int nInds, nVerts, nBones;
	
	file.read((char*)&nInds, sizeof(nInds));
	file.read((char*)&nVerts, sizeof(nVerts));
	file.read((char*)&nBones, sizeof(nBones));

	for (unsigned int n = 0; n < nBones; ++n)	//Skip past Bones section. TODO: care about this once bones are implemented
	{
		char padd[512];

		file.read(padd, 130);
		file.read(padd, padd[128]);
	}

	unsigned int* inds = new unsigned int[nInds];

	file.read((char*)inds, nInds * sizeof(unsigned int));

	unsigned int vertSize = 3;
	
	if (fmt[0])				//Normals
	{
		vertSize += 3;
	}
	
	if (fmt[1])				//Colors
	{
		vertSize += 4;
	}

	if (fmt[2])				//UV1
	{
		vertSize += 2;
	}

	if (fmt[3])				//UV2
	{
		vertSize += 2;
	}

	if (fmt[4])				//Weights
	{
		vertSize += 4;
	}

	vertSize *= sizeof(float);

	char* verts = new char[vertSize * nVerts];

	file.read(verts, vertSize * nVerts);

	file.close();

	GLuint VBO, IBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * nVerts, verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * nInds, inds, GL_STATIC_DRAW);

	return new Mesh(VBO, IBO, nVerts, nInds, inds, verts);
}

Mesh* LoadTEMMesh_0_9_1(std::ifstream& file)
{
	bool fmt[5];
	file.read((char*)fmt, sizeof(bool)* 5);

	unsigned int nInds, nVerts, nBones;

	file.read((char*)&nInds, sizeof(nInds));
	file.read((char*)&nVerts, sizeof(nVerts));
	file.read((char*)&nBones, sizeof(nBones));

	for (unsigned int n = 0; n < nBones; ++n)	//Skip past Bones section. TODO: care about this once bones are implemented
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

	unsigned int* inds = new unsigned int[nInds];

	file.read((char*)inds, nInds * sizeof(unsigned int));

	unsigned int vertSize = 3;

	if (fmt[0])				//Normals
	{
		vertSize += 3;
	}

	if (fmt[1])				//Colors
	{
		vertSize += 4;
	}

	if (fmt[2])				//UV1
	{
		vertSize += 2;
	}

	if (fmt[3])				//UV2
	{
		vertSize += 2;
	}

	if (fmt[4])				//Weights
	{
		vertSize += 4;
	}

	vertSize *= sizeof(float);

	char* verts = new char[vertSize * nVerts];

	file.read(verts, vertSize * nVerts);

	file.close();

	GLuint VBO, IBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * nVerts, verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* nInds, inds, GL_STATIC_DRAW);

	return new Mesh(VBO, IBO, nVerts, nInds, inds, verts, boundingBox);
}

Mesh* MeshAssetManager::Load(MeshKey key)
{
	try
	{
		std::ifstream file(key.name, std::ios::binary | std::ios::in);

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
	it->second.data->clear();
}