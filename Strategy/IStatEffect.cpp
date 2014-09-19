#include "IStatEffect.h"

bool IStatEffect::isApplicable()
{
	return true;
}

bool IStatEffect::hasEnded()
{
	if (!infinite && SDL_GetTicks() >= endTime) 
	{
		return true;
	}
	
	return false;
}