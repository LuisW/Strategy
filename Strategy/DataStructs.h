#pragma once

struct RenderSettings
{
	float renderDistance;
};

namespace TerrainSettings
{
	const float MinimumCellLength = 0.1f;
	const int CellsPerLeaf = 32;
	const int LoDs = 7;
	const float LoDDistances[] = { 0.0f, 2.0f, 12.8f, 27.6f, 64.0f, 130.0f, 268.8f, 539.6f, 1088.0f, 2178.0f, 4364.8f, 8731.6f, 17472.0f};
	const float LoDLerpPercentage = 0.5f;
	const unsigned int HeightMapRes = 129;
	const unsigned int HMapResWBorder = HeightMapRes + 4;
	const unsigned int HeightMapCnt = (1 << LoDs) * CellsPerLeaf / (HeightMapRes - 1);
	const unsigned int TileCacheSize = 50;
	const float mapScale = MinimumCellLength * HeightMapRes;
	const float Yscale = 200.0f;
	const unsigned int hmap = 1000;
	extern const char* hmapPrefix;
	extern const char* hmapName;
};