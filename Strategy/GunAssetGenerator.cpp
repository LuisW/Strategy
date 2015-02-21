#include "GunAssetGenerator.h"

enum GunType {GT_AssaultRifle, GT_Shotgun, GT_Unused};

#define GripLen 0.15f
#define HandLen 0.15f
#define MagLen 0.15f
#define MaxGripPos 0.33f
#define MaxHandPos 0.6f
#define MinRecLen (GripLen+HandLen+MagLen)

#define invsqrt3 0.5773502692f
#define invsqrt2 0.7071067812f

MeshAsset_const GunAssetGenerator::GenerateAssaultRifle(unsigned int seed)
{
	MeshKey key = MeshKey(genAssetKey("ProceduralAssaultRifle:", seed));
	if (AssetManager::hasAsset<Mesh>(key))
	{
		return AssetManager::getAsset<Mesh>(key);
	}

	float baseLen = randFltWeighted1(seed, 0.70f, 0.15f);
	float receiverLen = randFltWeighted1(seed * 7, (MinRecLen + baseLen) * 0.5f, (baseLen - MinRecLen) * 0.5f);
	float stockLen = randFltWeighted1(seed * 3, 0.16f, 0.04f);
	float magPlace;
	float gripPlace;
	float handPlace;

	if (randBool(seed * 13, 0.5f))	//mag infront of grip
	{
		gripPlace = randFlt(seed * 11, 0.0f, glm::min(MaxGripPos - stockLen, receiverLen - MinRecLen));
		handPlace = randFlt(seed * 13, gripPlace + GripLen + MagLen, glm::min(receiverLen - HandLen, MaxHandPos - stockLen));
		magPlace = randFlt(seed * 17, gripPlace + GripLen, handPlace - MagLen);
	}
	else
	{
		gripPlace = randFlt(seed * 11, MagLen, glm::min(MaxGripPos - stockLen, receiverLen - GripLen+HandLen));
		handPlace = randFlt(seed * 13, gripPlace + GripLen, glm::min(receiverLen - HandLen, MaxHandPos - stockLen));
		magPlace = randFlt(seed * 17, 0.0f, gripPlace - MagLen);
	}

	ProcMesh pmesh;
	float bbAttach;

	GenerateReceiver(seed * 19, stockLen, glm::max(gripPlace + GripLen, magPlace + MagLen), receiverLen, baseLen - receiverLen, pmesh, bbAttach);
	GenerateStock(seed * 23, stockLen, pmesh);
	GenerateMag(seed * 29, stockLen + magPlace, bbAttach, pmesh);
	GenerateGrip(seed * 37, stockLen + gripPlace, bbAttach, pmesh);

	pmesh.CalcNormals();
	return AssetManager::addAsset<Mesh>(key, pmesh.makeMesh());
}

void mkCube(ProcMesh& pmesh, glm::vec2 x, glm::vec2 y, glm::vec2 z)
{
	pmesh.setOffset();

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				pmesh.AddVertex(x[i], y[j], z[k]);
			}
		}
	}

	//Left
	pmesh.AddQuadOffset(0, 1, 3, 2, pmesh.newSurface());

	//Front
	pmesh.AddQuadOffset(0, 2, 6, 4, pmesh.newSurface());

	//Top
	pmesh.AddQuadOffset(2, 3, 7, 6, pmesh.newSurface());

	//Right
	pmesh.AddQuadOffset(4, 6, 7, 5, pmesh.newSurface());

	//Back
	pmesh.AddQuadOffset(7, 3, 1, 5, pmesh.newSurface());

	//Bottom
	pmesh.AddQuadOffset(0, 4, 5, 1, pmesh.newSurface());
}

void mkMagBlock(ProcMesh& pmesh, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec2 x, bool renderBot)
{
	pmesh.setOffset();

	for (int i = 0; i < 2; i++)
	{
		pmesh.AddVertex(x[i], p1.x, p1.y);
		pmesh.AddVertex(x[i], p2.x, p2.y);
		pmesh.AddVertex(x[i], p4.x, p4.y);
		pmesh.AddVertex(x[i], p3.x, p3.y);
	}

	//Left
	pmesh.AddQuadOffset(0, 1, 3, 2, pmesh.newSurface());

	//Front
	pmesh.AddQuadOffset(0, 2, 6, 4, pmesh.newSurface());

	//Top
	//pmesh.AddQuadOffset(2, 3, 7, 6, pmesh.newSurface());

	//Right
	pmesh.AddQuadOffset(4, 6, 7, 5, pmesh.newSurface());

	//Back
	pmesh.AddQuadOffset(7, 3, 1, 5, pmesh.newSurface());

	//Bottom
	if (renderBot)
	{
		pmesh.AddQuadOffset(0, 4, 5, 1, pmesh.newSurface());
	}
}

void mkNGon(ProcMesh& mesh, float pos, float rad, unsigned int ngon, bool off, glm::vec2 z, bool smooth)
{
	mesh.setOffset();

	mesh.AddVertex(0.0f, pos, z.x);
	mesh.AddVertex(0.0f, pos, z.y);

	float stepangle = 2.0f * M_PI / ngon;

	glm::vec2 step(glm::cos(stepangle), glm::sin(stepangle));

	glm::vec2 ngonvert;
	
	if (off)
	{
		ngonvert = glm::vec2(glm::cos(stepangle*0.5f)*rad, glm::sin(stepangle*0.5f)*rad);
	}
	else
	{
		ngonvert = glm::vec2(rad, 0.0f);
	}

	mesh.AddVertex(ngonvert.x, ngonvert.y + pos, z.x);
	mesh.AddVertex(ngonvert.x, ngonvert.y + pos, z.y);

	for (unsigned int n = 1; n < ngon; n++)
	{
		ngonvert = glm::vec2(ngonvert.x*step.x - ngonvert.y*step.y, ngonvert.x*step.y + ngonvert.y*step.x);

		mesh.AddVertex(ngonvert.x, ngonvert.y + pos, z.x);
		mesh.AddVertex(ngonvert.x, ngonvert.y + pos, z.y);
	}

	unsigned int front = mesh.newSurface();
	unsigned int back = mesh.newSurface();
	unsigned int hull = mesh.newSurface();

	for (unsigned int n = 2; n < ngon * 2; n += 2)
	{
		if (smooth)
		{
			mesh.AddQuadOffset(n, 2 + n, 3 + n, 1 + n, hull);
		}
		else
		{
			mesh.AddQuadOffset(n, 2 + n, 3 + n, 1 + n, mesh.newSurface());
		}
		mesh.AddTriangleOffset(0, 2 + n, n, back);
		mesh.AddTriangleOffset(1, 1 + n, 3 + n, front);
	}

	if (smooth)
	{
		mesh.AddQuadOffset(2*ngon, 2, 3, 2*ngon+1, hull);
	}
	else
	{
		mesh.AddQuadOffset(2*ngon, 2, 3, 1 + 2*ngon, mesh.newSurface());
	}
	mesh.AddTriangleOffset(0, 2, 2*ngon, back);
	mesh.AddTriangleOffset(1, 1 + 2*ngon, 3, front);
}

struct RecCut
{
	float topB;
	float botB;
	float baseZ;
	float topM;
	float botM;

	float func(float y)
	{
		if (y < botB)
		{
			return (botB - y) * botM + baseZ;
		}
		
		if (y > topB)
		{
			return (y - topB) * topM + baseZ;
		}

		return baseZ;
	}
};

struct MGonBlockData
{
	glm::vec2 pos;
	glm::vec2 rad;
	glm::uvec2 ngon;
	glm::bvec2 off;
	glm::bvec2 smooth;

	RecCut fcut;
	RecCut bcut;
};

void mkNGonBlock(ProcMesh& mesh,MGonBlockData& d)
{
	mesh.setOffset();

	mesh.AddVertex(0.0f, d.pos.x, d.bcut.func(d.pos.x));
	mesh.AddVertex(0.0f, d.pos.x, d.fcut.func(d.pos.x));
	mesh.AddVertex(0.0f, d.pos.y, d.bcut.func(d.pos.y));
	mesh.AddVertex(0.0f, d.pos.y, d.fcut.func(d.pos.y));

	glm::uvec2 cnt;

	for (int i = 0; i < 2; i++)
	{
		float stepangle = 2.0f * M_PI / d.ngon[i];

		glm::vec2 step(glm::cos(stepangle), glm::sin(stepangle));
		glm::vec2 halfstep(glm::cos(0.5f * stepangle), glm::sin(0.5f * stepangle));
		bool onAxis = (d.ngon[i] % 4 == 0) != d.off[i];

		glm::vec2 ngonvert = glm::vec2(d.rad[i], 0.0f);

		if (i == 1)
		{
			ngonvert.x *= -1.0f;
		}

		if (!onAxis)
		{
			mesh.AddVertex(ngonvert.x * halfstep.x, d.pos[i], d.bcut.func(d.pos[i]));
			mesh.AddVertex(ngonvert.x * halfstep.x, d.pos[i], d.fcut.func(d.pos[i]));

			ngonvert = glm::vec2(ngonvert.x*halfstep.x - ngonvert.y*halfstep.y, ngonvert.x*halfstep.y + ngonvert.y*halfstep.x);
		}
		else
		{
			mesh.AddVertex(ngonvert.x, d.pos[i], d.bcut.func(d.pos[i]));
			mesh.AddVertex(ngonvert.x, d.pos[i], d.fcut.func(d.pos[i]));

			ngonvert = glm::vec2(ngonvert.x*step.x - ngonvert.y*step.y, ngonvert.x*step.y + ngonvert.y*step.x);
		}

		cnt[i] = d.ngon[i] / 2;
		if (!onAxis)
			cnt[i]--;

		for (unsigned int n = 0; n < cnt[i]; n++)
		{
			mesh.AddVertex(ngonvert.x, ngonvert.y + d.pos[i], d.bcut.func(ngonvert.y + d.pos[i]));
			mesh.AddVertex(ngonvert.x, ngonvert.y + d.pos[i], d.fcut.func(ngonvert.y + d.pos[i]));

			ngonvert = glm::vec2(ngonvert.x*step.x - ngonvert.y*step.y, ngonvert.x*step.y + ngonvert.y*step.x);
		}

		if (!onAxis)
		{
			mesh.AddVertex(ngonvert.x, ngonvert.y + d.pos[i], d.bcut.func(ngonvert.y + d.pos[i]));
			mesh.AddVertex(ngonvert.x, ngonvert.y + d.pos[i], d.fcut.func(ngonvert.y + d.pos[i]));

			ngonvert = glm::vec2(ngonvert.x*halfstep.x - ngonvert.y*halfstep.y, ngonvert.x*halfstep.y + ngonvert.y*halfstep.x);

			mesh.AddVertex(ngonvert.x * halfstep.x, ngonvert.y + d.pos[i], d.bcut.func(ngonvert.y + d.pos[i]));
			mesh.AddVertex(ngonvert.x * halfstep.x, ngonvert.y + d.pos[i], d.fcut.func(ngonvert.y + d.pos[i]));

			cnt[i] += 2;
		}
	}
		
	unsigned int front = mesh.newSurface();
	unsigned int back = mesh.newSurface();
	unsigned int hull = mesh.newSurface();

	for (unsigned int n = 4; n < cnt[0] * 2 + 4; n += 2)
	{
		if (d.smooth[0])
		{
			mesh.AddQuadOffset(n, 2 + n, 3 + n, 1 + n, hull);
		}
		else
		{
			mesh.AddQuadOffset(n, 2 + n, 3 + n, 1 + n, mesh.newSurface());
		}
		mesh.AddTriangleOffset(0, 2 + n, n, back);
		mesh.AddTriangleOffset(1, 1 + n, 3 + n, front);
	}

	for (unsigned int n = 6 + cnt[0] * 2; n < 6 + (cnt[0] + cnt[1]) * 2; n += 2)
	{
		if (d.smooth[1])
		{
			mesh.AddQuadOffset(n, 2 + n, 3 + n, 1 + n, hull);
		}
		else
		{
			mesh.AddQuadOffset(n, 2 + n, 3 + n, 1 + n, mesh.newSurface());
		}
		mesh.AddTriangleOffset(2, 2 + n, n, back);
		mesh.AddTriangleOffset(3, 1 + n, 3 + n, front);
	}

#define tfirst 4
#define tlast 4 + cnt[0] * 2
#define bfirst 6 + cnt[0] * 2
#define blast 6 + (cnt[0] + cnt[1]) * 2

	mesh.AddQuadOffset(2, bfirst, tlast, 0, front);
	mesh.AddQuadOffset(tfirst, blast, 2, 0, front);

	mesh.AddQuadOffset(bfirst + 1, 3, 1, tlast + 1, back);
	mesh.AddQuadOffset(3, blast + 1, tfirst + 1, 1, back);

	mesh.AddQuadOffset(tfirst, tfirst + 1, blast + 1, blast, mesh.newSurface());
	mesh.AddQuadOffset(tlast + 1, tlast, bfirst, bfirst + 1, mesh.newSurface());
}

void GunAssetGenerator::GenerateGrip(unsigned int seed, float pos, float y, ProcMesh& pmesh)
{
	mkCube(pmesh, glm::vec2(-0.025f, 0.025f), glm::vec2(y-0.15f, y), glm::vec2(pos, pos + GripLen));
}

void GunAssetGenerator::GenerateMag(unsigned int seed, float pos, float y, ProcMesh& pmesh)
{
	if (randBool(seed * 2, 0.5f)) // curved
	{
		float len = randFlt(seed * 3, 0.06f, 0.09f);
		float rad = randFlt(seed * 5, 0.5f, 1.0f);
		float fullLen = randFlt(seed * 7, len + 0.005f, 0.11f);

		float stepangle = 0.03f / rad;
		pos += (MagLen - fullLen) * 0.5f;
		float fullangle = glm::acos((rad + len - fullLen) / rad);
		float off = pos + len + rad;

		glm::vec2 step(glm::cos(stepangle), glm::sin(stepangle));
		glm::vec2 ngonvert = glm::vec2(-1.0f, 0.0f);

		pmesh.setOffset();

		float angle = 0.0f;
		unsigned int splits = 0;
		while (angle < fullangle)
		{
			pmesh.AddVertex(-0.01f, ngonvert.y * (rad + len) + y, ngonvert.x * (rad + len) + off);
			pmesh.AddVertex(-0.01f, ngonvert.y * rad + y, ngonvert.x * rad + off);
			pmesh.AddVertex(0.01f, ngonvert.y * rad + y, ngonvert.x * rad + off);
			pmesh.AddVertex(0.01f, ngonvert.y * (rad + len) + y, ngonvert.x * (rad + len) + off);
			angle += stepangle;
			splits++;
			ngonvert = glm::vec2(ngonvert.x*step.x - ngonvert.y*step.y, ngonvert.x*step.y + ngonvert.y*step.x);
		}

		splits--;

		unsigned int side1 = pmesh.newSurface();
		unsigned int side2 = pmesh.newSurface();
		unsigned int front = pmesh.newSurface();
		unsigned int back = pmesh.newSurface();

		for (int n = 0; n < splits; n++)
		{
			pmesh.AddQuadOffset(4 * n, 4 * n + 4, 4 * n + 5, 4 * n + 1, side1);
			pmesh.AddQuadOffset(4 * n + 5, 4 * n + 6, 4 * n + 2, 4 * n + 1, front);
			pmesh.AddQuadOffset(4 * n + 3, 4 * n + 2, 4 * n + 6, 4 * n + 7, side2);
			pmesh.AddQuadOffset(4 * n + 7, 4 * n + 4, 4 * n, 4 * n + 3, back);
		}

		pmesh.AddQuadOffset(4 * splits, 4 * splits + 3, 4 * splits + 2, 4 * splits + 1, pmesh.newSurface());
	}
	else	// shifted
	{
		float shift = 0.0f;
		float height = randFlt(seed * 3, 0.1f, 0.25f);
		float len = randFlt(seed * 5, 0.07f, 0.1f);

		if (randBool(seed * 7, 0.5f))
		{
			shift = randFlt(seed * 11, 0.0f, 0.05f);

			if (shift + len > (MagLen - 0.04f))
			{
				shift = (MagLen - 0.04f) - len;
			}
			pos += (MagLen - shift - len) *0.5f;
		}
		else
		{
			pos += (MagLen - len) *0.5f;
		}

		float frontShift = 0.0f;
		
		if (randBool(seed * 13, 0.5f))
		{
			frontShift = randFlt(seed * 17, 0.0f, 0.02f);
		}

		pmesh.setOffset();

		glm::vec2 p1 = glm::vec2(y - height, pos + shift);
		glm::vec2 p2 = glm::vec2(y - height + frontShift, pos + shift + len);
		glm::vec2 p3 = glm::vec2(y, pos + len);
		glm::vec2 p4 = glm::vec2(y, pos);

		pmesh.AddVertex(-0.01f, p1.x, p1.y);
		pmesh.AddVertex(-0.01f, p2.x, p2.y);
		pmesh.AddVertex(-0.01f, p4.x, p4.y);
		pmesh.AddVertex(-0.01f, p3.x, p3.y);

		pmesh.AddVertex(0.01f, p1.x, p1.y);
		pmesh.AddVertex(0.01f, p2.x, p2.y);
		pmesh.AddVertex(0.01f, p4.x, p4.y);
		pmesh.AddVertex(0.01f, p3.x, p3.y);

		//Left
		pmesh.AddQuadOffset(0, 1, 3, 2, pmesh.newSurface());

		//Front
		pmesh.AddQuadOffset(0, 2, 6, 4, pmesh.newSurface());

		//Top
		//pmesh.AddQuadOffset(2, 3, 7, 6, pmesh.newSurface());

		//Right
		pmesh.AddQuadOffset(4, 6, 7, 5, pmesh.newSurface());

		//Back
		pmesh.AddQuadOffset(7, 3, 1, 5, pmesh.newSurface());

		//Bottom
		pmesh.AddQuadOffset(0, 4, 5, 1, pmesh.newSurface());
	}
}

void GunAssetGenerator::GenerateStock(unsigned int seed, float len, ProcMesh& pmesh)
{
	mkCube(pmesh, glm::vec2(-0.025f, 0.025f), glm::vec2(-0.225f, 0.075f), glm::vec2(0.0f, len));
}

#define minBarrelRadius 0.01f
#define maxBarrelRadius 0.02f
#define minTubeRadius 0.005f
#define minTubeDist 0.0f
#define maxTubeDist 0.02f
#define maxReceiverRadius 0.02f
#define minReceiverRadius 0.005f

enum SplitStyle {ST_Full, ST_FrontBack, ST_TopBot,  ST_None};
enum Section { TB, TF, BF, BB };
const unsigned int validNGons[] = { 6, 4, 8, 20 };

void GunAssetGenerator::GenerateReceiver(unsigned int seed, float pos, float len1, float len2, float barrelLen, ProcMesh& pmesh, float& BBAttach)	//pos: start, len1: end of first part, len: end of second part
{
	SplitStyle split = (SplitStyle)randuint(seed * 2, ST_Full, ST_None);
	bool stockJoin = randBool(seed * 3, 0.5f);

	float barrelRadius = randFlt(seed * 5, minBarrelRadius, maxBarrelRadius);

	bool gastube = randBool(seed * 7, 0.7f);
	float tubeRadius = 0.0f;
	float tubeLen = 0.0f;
	float sndMid = 0.0f;

	if (gastube)
	{
		tubeRadius = randFlt(seed * 19, minTubeRadius, barrelRadius);
		tubeLen = randFlt(seed * 23, 0.0f, barrelLen);
		sndMid = randBool(seed * 29, 0.5f) ? 1.0f : -1.0f;
		sndMid *= randFlt(seed * 31, barrelRadius + tubeRadius + minTubeDist, barrelRadius + tubeRadius + maxTubeDist);
	}

	unsigned int ngon[4];
	float center[4];
	float startradius[4];
	float endradius[4];

	float botMid = glm::min(sndMid, 0.0f);
	float botRad = botMid == 0.0f ? barrelRadius : tubeRadius;
	float topMid = glm::max(sndMid, 0.0f);
	float topRad = topMid == 0.0f ? barrelRadius : tubeRadius;

	ngon[BB] = 4;

	switch (split)
	{
	case ST_Full:
		ngon[TB] = validNGons[randuint(seed * 11, 1, 3)];
		ngon[TF] = validNGons[randuint(seed * 13, 0, 3)];
		ngon[BF] = validNGons[randuint(seed * 17, 0, 3)];

		center[BB] = randFlt(seed * 37, botMid, topMid);
		center[TB] = randFlt(seed * 41, center[BB], topMid);

		center[BF] = randFlt(seed * 43, botMid, topMid);
		center[TF] = randFlt(seed * 47, center[BF], topMid);

		startradius[BB] = (center[BB] - botMid + botRad) / glm::cos(M_PI_f / ngon[BB]) + randFlt(seed * 53, minReceiverRadius, maxReceiverRadius);
		startradius[BF] = (center[BF] - botMid + botRad) / glm::cos(M_PI_f / ngon[BF]) + randFlt(seed * 59, minReceiverRadius, maxReceiverRadius);
		startradius[TB] = (topMid - center[TB] + topRad) / glm::cos(M_PI_f / ngon[TB]) + randFlt(seed * 61, minReceiverRadius, maxReceiverRadius);
		startradius[TF] = (topMid - center[TF] + topRad) / glm::cos(M_PI_f / ngon[TF]) + randFlt(seed * 67, minReceiverRadius, maxReceiverRadius);
		break;
	case ST_FrontBack:
		ngon[TB] = validNGons[randuint(seed * 11, 1, 3)];
		ngon[TF] = validNGons[randuint(seed * 13, 0, 3)];
		ngon[BF] = ngon[TF];

		center[BB] = randFlt(seed * 37, botMid, topMid);
		center[TB] = randFlt(seed * 41, center[BB], topMid);

		center[BF] = randFlt(seed * 43, botMid, topMid);
		center[TF] = randFlt(seed * 47, center[BF], topMid);

		startradius[BB] = (center[BB] - botMid + botRad) / glm::cos(M_PI_f / ngon[BB]);
		startradius[BF] = (center[BF] - botMid + botRad) / glm::cos(M_PI_f / ngon[BF]);
		startradius[TB] = (topMid - center[TB] + topRad) / glm::cos(M_PI_f / ngon[TB]);
		startradius[TF] = (topMid - center[TF] + topRad) / glm::cos(M_PI_f / ngon[TF]);

		startradius[BB] = startradius[BF] = glm::max(startradius[BB], startradius[BF]) + randFlt(seed * 53, minReceiverRadius, maxReceiverRadius);
		startradius[TB] = startradius[TF] = glm::max(startradius[TB], startradius[TF]) + randFlt(seed * 59, minReceiverRadius, maxReceiverRadius);
		break;
	case ST_TopBot:
		ngon[TB] = validNGons[randuint(seed * 11, 1, 3)];
		ngon[TF] = ngon[TB];
		ngon[BF] = validNGons[randuint(seed * 13, 0, 3)];

		center[BB] = randFlt(seed * 37, botMid, topMid);
		center[BF] = randFlt(seed * 41, botMid, topMid);

		center[TB] = center[TF] = randFlt(seed * 41, glm::max(center[BB], center[BF]), topMid);

		startradius[BB] = (center[BB] - botMid + botRad) / glm::cos(M_PI_f / ngon[BB]) + randFlt(seed * 53, minReceiverRadius, maxReceiverRadius);
		startradius[BF] = (center[BF] - botMid + botRad) / glm::cos(M_PI_f / ngon[BF]) + randFlt(seed * 59, minReceiverRadius, maxReceiverRadius);
		startradius[TB] = startradius[TF] = (topMid - center[TB] + topRad) / glm::cos(M_PI_f / ngon[TB]) + randFlt(seed * 61, minReceiverRadius, maxReceiverRadius);
		break;
	case ST_None:
		ngon[TB] = validNGons[randuint(seed * 11, 1, 3)];
		ngon[TF] = ngon[BF] = ngon[BB] = ngon[TB];

		center[BB] = randFlt(seed * 37, botMid, topMid);
		center[TB] = randFlt(seed * 41, center[BB], topMid);

		center[BF] = randFlt(seed * 43, botMid, topMid);
		center[TF] = randFlt(seed * 47, center[BF], topMid);

		startradius[BB] = (center[BB] - botMid + botRad) / glm::cos(M_PI_f / ngon[BB]) + randFlt(seed * 53, minReceiverRadius, maxReceiverRadius);
		startradius[BF] = (center[BF] - botMid + botRad) / glm::cos(M_PI_f / ngon[BF]) + randFlt(seed * 59, minReceiverRadius, maxReceiverRadius);
		startradius[TB] = (topMid - center[TB] + topRad) / glm::cos(M_PI_f / ngon[TB]) + randFlt(seed * 61, minReceiverRadius, maxReceiverRadius);
		startradius[TF] = (topMid - center[TF] + topRad) / glm::cos(M_PI_f / ngon[TF]) + randFlt(seed * 67, minReceiverRadius, maxReceiverRadius);
		break;
	default:
		break;
	}

	RecCut backCut;
	RecCut midCut;
	RecCut frontCut;

	backCut.baseZ = pos;
	midCut.baseZ = pos + len1;
	frontCut.baseZ = pos + len2;

	backCut.topB = midCut.topB = frontCut.topB = INFINITY;
	backCut.botB = midCut.botB = frontCut.botB = -INFINITY;

	if (randBool(seed * 71, 0.5f))
	{
		backCut.botB = randFlt(seed * 73, botMid - maxReceiverRadius, topMid + maxReceiverRadius);
		backCut.botB = randFlt(seed * 79, botMid, topMid + maxReceiverRadius);
		backCut.botM = randFlt(seed * 83, -invsqrt3, invsqrt3);
		backCut.topM = randFlt(seed * 89, -invsqrt3, invsqrt3);
	}

	if (randBool(seed * 97, 0.5f))
	{
		midCut.botB = randFlt(seed * 101, botMid - maxReceiverRadius, topMid + maxReceiverRadius);
		midCut.botB = randFlt(seed * 103, botMid, topMid + maxReceiverRadius);
		midCut.botM = randFlt(seed * 107, -invsqrt3, invsqrt3);
		midCut.topM = randFlt(seed * 109, -invsqrt3, invsqrt3);
	}

	if (randBool(seed * 113, 0.5f))
	{
		frontCut.botB = randFlt(seed * 127, botMid - maxReceiverRadius, topMid + maxReceiverRadius);
		frontCut.botB = randFlt(seed * 131, botMid, topMid + maxReceiverRadius);
		frontCut.botM = randFlt(seed * 137, -invsqrt3, invsqrt3);
		frontCut.topM = randFlt(seed * 139, -invsqrt3, invsqrt3);
	}

	MGonBlockData blockdata;

	switch (split)
	{
	case ST_Full:
		blockdata.pos = glm::vec2(topMid, botMid);
		blockdata.rad = glm::vec2(startradius[TF], startradius[BF]);
		blockdata.ngon = glm::uvec2(ngon[TF], ngon[BF]);
		blockdata.off = glm::bvec2(true);
		blockdata.bcut = midCut;
		blockdata.fcut = frontCut;
		blockdata.smooth = glm::bvec2(ngon[TF] == 20, ngon[BF] == 20);


		mkNGonBlock(pmesh, blockdata);

		blockdata.pos = glm::vec2(topMid, botMid);
		blockdata.rad = glm::vec2(startradius[TB], startradius[BB]);
		blockdata.ngon = glm::uvec2(ngon[TB], ngon[BB]);
		blockdata.off = glm::bvec2(true);
		blockdata.bcut = backCut;
		blockdata.fcut = midCut;
		blockdata.smooth = glm::bvec2(ngon[TB] == 20, ngon[BB] == 20);

		mkNGonBlock(pmesh, blockdata);
		break;
	case ST_FrontBack:
		blockdata.pos = glm::vec2(topMid, botMid);
		blockdata.rad = glm::vec2(startradius[TF], startradius[BF]);
		blockdata.ngon = glm::uvec2(ngon[TF], ngon[BF]);
		blockdata.off = glm::bvec2(true);
		blockdata.bcut = midCut;
		blockdata.fcut = frontCut;
		blockdata.smooth = glm::bvec2(ngon[TF] == 20, ngon[BF] == 20);

		mkNGonBlock(pmesh, blockdata);

		blockdata.pos = glm::vec2(topMid, botMid);
		blockdata.rad = glm::vec2(startradius[TB], startradius[BB]);
		blockdata.ngon = glm::uvec2(ngon[TB], ngon[BB]);
		blockdata.off = glm::bvec2(true);
		blockdata.bcut = backCut;
		blockdata.fcut = midCut;
		blockdata.smooth = glm::bvec2(ngon[TB] == 20, ngon[BB] == 20);

		mkNGonBlock(pmesh, blockdata);
		break;
	case ST_TopBot:
		blockdata.pos = glm::vec2(topMid, botMid);
		blockdata.rad = glm::vec2(startradius[TF], startradius[BF]);
		blockdata.ngon = glm::uvec2(ngon[TF], ngon[BF]);
		blockdata.off = glm::bvec2(true);
		blockdata.bcut = backCut;
		blockdata.fcut = frontCut;
		blockdata.smooth = glm::bvec2(ngon[TF] == 20, ngon[BF] == 20);

		mkNGonBlock(pmesh, blockdata);
		break;
	case ST_None:
		blockdata.pos = glm::vec2(topMid, botMid);
		blockdata.rad = glm::vec2(startradius[TF], startradius[BF]);
		blockdata.ngon = glm::uvec2(ngon[TF], ngon[BF]);
		blockdata.off = glm::bvec2(true);
		blockdata.bcut = backCut;
		blockdata.fcut = frontCut;
		blockdata.smooth = glm::bvec2(ngon[TF] == 20, ngon[BF] == 20);

		mkNGonBlock(pmesh, blockdata);
	default:
		break;
	}

	mkNGon(pmesh, 0.0f, barrelRadius, 20, false, glm::vec2(pos + len2 - ( 2.0f * maxReceiverRadius + maxTubeDist) * invsqrt3, pos + len2 + barrelLen), true);

	if (gastube)
	{
		mkNGon(pmesh, sndMid, tubeRadius, 20, false, glm::vec2(pos + len2 - (2.0f * maxReceiverRadius + maxTubeDist) * invsqrt3, pos + len2 + tubeLen), true);
	}

	BBAttach = botMid - startradius[BB] * glm::cos(M_PI_f / ngon[BB]);
}

MeshAsset_const GunAssetGenerator::GenerateGun(unsigned int seed)
{
	return GenerateAssaultRifle(seed);	
}