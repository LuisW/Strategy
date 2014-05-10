#pragma once

struct RenderSettings
{
	float renderDistance;
};

namespace TerrainSettings
{
	const float MinimumCellLength = 0.01f;
	const int CellsPerLeaf = 32;
	const int LoDs = 8;
};