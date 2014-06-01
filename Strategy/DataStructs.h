#pragma once

struct RenderSettings
{
	float renderDistance;
};

namespace TerrainSettings
{
	const float MinimumCellLength = 0.1f;
	const int CellsPerLeaf = 32;
	const int LoDs = 12;
	const float LoDDistances[] = { 0.0f, 2.0f, 12.8f, 27.6f, 64.0f, 130.0f, 268.8f, 539.6f, 1088.0f, 2178.0f, 4364.8f, 8731.6f, 17472.0f};
	const float LoDLerpPercentage = 0.5f;
};