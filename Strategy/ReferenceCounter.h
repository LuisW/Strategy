#pragma once

class ReferenceCounter
{
private:
	unsigned int refCnt;

public:
	ReferenceCounter(unsigned int init) : refCnt(init)
	{}

	ReferenceCounter() : refCnt(0)
	{}

	inline bool IsReferenced()
	{
		return refCnt > 0;
	}

	inline void AddReference()
	{
		refCnt++;
	}

	inline bool DeleteReference()
	{
		if (refCnt > 0)
		{
			refCnt--;
		}

		return IsReferenced();
	}
};