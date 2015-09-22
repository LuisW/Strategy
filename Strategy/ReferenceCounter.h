#pragma once

class ReferenceCounter
{
private:
	unsigned int m_refCnt;

public:
	ReferenceCounter(unsigned int init) : m_refCnt(init)
	{}

	ReferenceCounter() : m_refCnt(0)
	{}

	inline bool IsReferenced()
	{
		return m_refCnt > 0;
	}

	inline void AddReference()
	{
		m_refCnt++;
	}

	inline bool DeleteReference()
	{
		if (m_refCnt > 0)
		{
			m_refCnt--;
		}

		return IsReferenced();
	}
};