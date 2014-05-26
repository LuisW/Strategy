#include "Terrain.h"
#include "ei/glm/glm.hpp"

void Terrain::makeMesh(unsigned int cells, float celllen, Mesh& mesh)
{
	unsigned int vertlen = (cells + 1) * (cells + 1) * 3;
	unsigned int indlen = 3 * 6 * cells * cells - 9 * cells / 2;	

	float *verts = new float[vertlen];
	unsigned int *inds = new unsigned int[indlen];


	int v = -1;
	for (unsigned int x = 0; x < cells + 1; x++)
	{
		for (unsigned int z = 0; z < cells + 1; z++)
		{
			verts[++v] = x * celllen;
			verts[++v] = 0;
			verts[++v] = z * celllen;
		}
	}

	int ind = -1;																	
																						
	for (unsigned int x = 0; x < cells; x++)											
	{																						
		for (unsigned int z = 0; z < cells; z++)										
		{																	
			inds[++ind] = (x + 1) * (cells + 1) + z;
			inds[++ind] =  x      * (cells + 1) + z;												
			inds[++ind] = (x + 1) * (cells + 1) + z + 1;							

			inds[++ind] = (x + 1) * (cells + 1) + z + 1;
			inds[++ind] =  x      * (cells + 1) + z;
			inds[++ind] =  x      * (cells + 1) + z + 1;
		}
	}

	for (unsigned int z = 0; z < cells; z+=2)
	{
		inds[++ind] = cells + 1 + z;
		inds[++ind] = z;
		inds[++ind] = cells + 1 + z + 1;

		inds[++ind] = cells + 1 + z + 1;
		inds[++ind] = z;	
		inds[++ind] = z + 2;

		inds[++ind] = cells + 1 + z + 1;
		inds[++ind] = z + 2;
		inds[++ind] = cells + 1 + z + 2;
	}

	for (unsigned int x = 1; x < cells; x++)
	{
		for (unsigned int z = 0; z < cells; z++)
		{
			inds[++ind] = (x + 1) * (cells + 1) + z;
			inds[++ind] =  x      * (cells + 1) + z;			
			inds[++ind] = (x + 1) * (cells + 1) + z + 1;

			inds[++ind] = (x + 1) * (cells + 1) + z + 1;
			inds[++ind] =  x      * (cells + 1) + z;			
			inds[++ind] =  x      * (cells + 1) + z + 1;
		}
	}

	for (unsigned int z = 0; z < cells; z += 2)
	{
		if (z != 0)
		{
			inds[++ind] = cells + 1 + z;
			inds[++ind] = z;			
			inds[++ind] = cells + 1 + z + 1;
		}

		inds[++ind] = cells + 1 + z + 1;
		inds[++ind] = z;
		inds[++ind] = z + 2;

		inds[++ind] = cells + 1 + z + 1;
		inds[++ind] = z + 2;
		inds[++ind] = cells + 1 + z + 2;
	}

	for (unsigned int x = 0; x < cells; x += 2)
	{
		if (x != 0)
		{
			inds[++ind] = (x + 1) * (cells + 1) + 1;
			inds[++ind] = x * (cells + 1);
			inds[++ind] = x * (cells + 1) + 1;
		}

		inds[++ind] = (x + 2) * (cells + 1);
		inds[++ind] = x * (cells + 1);
		inds[++ind] = (x + 1) * (cells + 1) + 1;

		inds[++ind] = (x + 2) * (cells + 1) + 1;
		inds[++ind] = (x + 2) * (cells + 1);
		inds[++ind] = (x + 1) * (cells + 1) + 1;
	}

	for (unsigned int x = 1; x < cells; x++)
	{
		for (unsigned int z = 1; z < cells; z++)
		{
			inds[++ind] = (x + 1) * (cells + 1) + z;
			inds[++ind] =  x      * (cells + 1) + z;
			inds[++ind] = (x + 1) * (cells + 1) + z + 1;

			inds[++ind] = (x + 1) * (cells + 1) + z + 1;
			inds[++ind] =  x      * (cells + 1) + z;
			inds[++ind] =  x      * (cells + 1) + z + 1;
		}
	}

	GLuint VBO = 0;
	GLuint IBO = 0;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indlen, inds, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertlen, verts, GL_STATIC_DRAW);

	mesh = Mesh(VBO, IBO, vertlen, indlen, inds, (char*)verts);
}