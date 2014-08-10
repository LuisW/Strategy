#pragma once

#include <vector>
#include "ei/glm/glm.hpp"

struct Map
{
	struct BiomeMapElem
	{
		glm::vec2 center;
		unsigned int moisture;
		unsigned int elevation;
	};

	struct FunctionMapElem
	{
		glm::vec2 center;
		bool playable;
		bool set;

		FunctionMapElem(const glm::vec2& _center, bool _playable) : center(_center), playable(_playable), set(false)
		{
		}

		FunctionMapElem() : center(), playable(false), set(false)
		{
		}
	};

	std::vector<BiomeMapElem> biomeMap;
	std::vector<FunctionMapElem> functionMap;
};