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
	const float LoDDistances[] = { 0.0f, 2.0f, 12.0f, 24.0f, 48.0f, 96.0f, 192.0f, 384.0f, 768.0f, 1536.0f, 3072.0f, 6144.0f, 12288.0f};
};