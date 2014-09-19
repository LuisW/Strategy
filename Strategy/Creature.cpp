#include "Creature.h"

unsigned int rand(unsigned int seed)
{
	seed = (seed ^ 61u) ^ (seed >> 16u);
	seed *= 9u;
	seed = seed ^ (seed >> 4u);
	seed *= 668265261u;
	return seed ^ (seed >> 15u);
}

void Creature::mkParams()
{
	hitpoints = rand(seed * 10) % 10000;
	speed = rand(seed * 20) % 1000;
	turnspeed = rand(seed * 30) % 360;
}