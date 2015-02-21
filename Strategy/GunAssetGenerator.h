#pragma once

#include <vector>
#include "AssetManager.h"
#include "ProcGenHelp.h"

class GunAssetGenerator
{
private:
	static MeshAsset_const GenerateAssaultRifle(unsigned int seed);

	static void GenerateGrip(unsigned int seed, float pos, float y, ProcMesh& pmesh);
	static void GenerateMag(unsigned int seed, float pos, float y, ProcMesh& pmesh);
	static void GenerateStock(unsigned int seed, float len, ProcMesh& pmesh);
	static void GenerateReceiver(unsigned int seed, float pos, float len1, float len2, float barrelLen, ProcMesh& pmesh, float& BBAttach);

public:
	static MeshAsset_const GenerateGun(unsigned int seed);
};