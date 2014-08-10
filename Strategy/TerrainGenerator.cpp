#include "TerrainGenerator.h"
#include "Map.h"
#include "DataStructs.h"

unsigned int rand(unsigned int seed)
{
	seed += 10;
	seed = (seed ^ 61u) ^ (seed >> 16u);
	seed *= 9u;
	seed = seed ^ (seed >> 4u);
	seed *= 668265261u;
	return seed ^ (seed >> 15u);
}

#define hmapres TerrainSettings::HeightMapRes
#define hexA 10.0f
#define playChance 50;
#define MapPaths 2

const glm::vec2 qAxis(0.0f, glm::sqrt(3.0f) * hexA);
const glm::vec2 rAxis(1.5f * hexA, -0.5f * glm::sqrt(3.0f) * hexA);
const glm::vec2 sAxis(1.5f * hexA, 0.5f * glm::sqrt(3.0f) * hexA);
const float rf = rAxis.y / rAxis.x;

glm::vec2 cartToQR(glm::vec2 p)
{																				//if qAxis,x != 0
	float r = p.x / rAxis.x;													//(p.x - qf * p.y) / (rAxis.x - qf * rAxis.y);
	float q = (p.y - rf * p.x) / (qAxis.y);										//(p.x - rf * p.y) / (qAxis.x - r * qAxis.y);

	return glm::vec2(r, q);
}

const int Neighbours[12] = { 1,  0, 
							 0,  1, 
							-1,  0, 
							 0, -1, 
							 1,  1, 
							-1, -1 };

void makePlayability(int xCells, Map map)
{
	for (int n = 0; n < MapPaths; n++)
	{
		int type = rand(n * xCells) % 4;

		glm::vec2 b1, b2;

		switch (type)
		{
		case 0:
			b1 = glm::vec2(rand(n + type + xCells) % hmapres, 0);
			break;
		case 1:
			b1 = glm::vec2(rand(n + type + xCells) % hmapres, hmapres);
			break;
		case 2:
			b1 = glm::vec2(0, rand(n + type + xCells) % hmapres);
			break;
		case 3:
			b1 = glm::vec2(hmapres, rand(n + type + xCells) % hmapres);
			break;
		}

		int type2 = rand(n * xCells * 100) % 4;

		if (type2 == type)
		{
			type2++;
			type2 %= 4;
		}

		switch (type2)
		{
		case 0:
			b2 = glm::vec2(rand(n + type2 + xCells) % hmapres, 0);
			break;
		case 1:
			b2 = glm::vec2(rand(n + type2 + xCells) % hmapres, hmapres);
			break;
		case 2:
			b2 = glm::vec2(0, rand(n + type2 + xCells) % hmapres);
			break;
		case 3:
			b2 = glm::vec2(hmapres, rand(n + type2 + xCells) % hmapres);
			break;
		}

		float dist = (b1 - b2).length();

		std::vector<glm::vec2> points(dist / 200 + 1);
		points[0] = b1;
		if (points.size() == 1)
		{
			points.push_back(b2);
		}
		else
		{
			points[points.size() - 1] = b2;
		}

		for (int n = 1; n < points.size() - 1; n++)
		{
			glm::vec2 dir = (b2 - b1) / dist;

			points[n] = 
		}
	}
}

GLuint TerrainGenerator::getHeightMap() const
{
	Map map;

	float s = ((float)hmapres);
	int xCells = (s) / rAxis.x;
	xCells+= 2;
	int yCells = (s) / qAxis.y;
	yCells += xCells / 2 + 2;


	map.functionMap.resize(xCells * yCells);

	for (float q = 0; q < yCells; q++)
	{
		float r = 0;
		while ((q * qAxis + r * rAxis).y >= -qAxis.y / 2.0f && (q * qAxis + r * rAxis).x <= s)
		{
			if ((q * qAxis + r * rAxis).y > s)
			{
				r++;
				continue;
			}

			float phi = ((float)(rand(q * yCells + r) % 1001)) * (360.0f / 1000.0f);
			float radius = 0.3f * hexA;

			map.functionMap[q * xCells + r] = Map::FunctionMapElem(q*qAxis + r*rAxis + glm::vec2(glm::cos(phi) * radius, glm::sin(phi) * radius), false);
			r++;
		}
	}

	map.functionMap[20 * xCells + 10].playable = true;
	map.functionMap[20 * xCells + 10].set = true;
	fillPlayability(10, 20, xCells, map);

	unsigned short* data = new unsigned short[hmapres * (hmapres + 1)];

	for (int n = 0; n < hmapres; n++)
	{
		for (int i = 0; i < hmapres; i++)
		{
			glm::vec2 p = glm::vec2(n, i);
			glm::vec2 qr = cartToQR(p);

			glm::vec2 dvec = map.functionMap[glm::floor(qr.y) * xCells + glm::floor(qr.x)].center - p;

			int minP = glm::floor(qr.y) * xCells + glm::floor(qr.x);
			float minD = glm::dot(dvec, dvec);

			int idx = glm::ceil(qr.y) * xCells + glm::floor(qr.x);
			dvec = map.functionMap[idx].center - p;
			float dist = glm::dot(dvec, dvec);

			if (dist < minD)
			{
				minP = idx;
				minD = dist;
			}

			idx = glm::floor(qr.y) * xCells + glm::ceil(qr.x);
			dvec = map.functionMap[idx].center - p;
			dist = glm::dot(dvec, dvec);

			if (dist < minD)
			{
				minP = idx;
				minD = dist;
			}

			idx = glm::ceil(qr.y) * xCells + glm::ceil(qr.x);
			dvec = map.functionMap[idx].center - p;
			dist = glm::dot(dvec, dvec);

			if (dist < minD)
			{
				minP = idx;
				minD = dist;
			}

			data[n * (hmapres + 1) + i] = 1000 * (int)(!map.functionMap[minP].playable);
		}
	}

	GLuint tex = 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16, hmapres, hmapres);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, hmapres, hmapres, GL_RED, GL_UNSIGNED_SHORT, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete[] data;

	return tex;
}