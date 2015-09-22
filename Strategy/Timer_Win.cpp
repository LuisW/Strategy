#include "Timer.h"
#include <Windows.h>

Timer::Timer()
{
	_LARGE_INTEGER lif;
	QueryPerformanceFrequency(&lif);

	m_freq = (double)lif.QuadPart;

	m_prevTime = getCurrent();
}

__int64 Timer::getCurrent()
{
	_LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

void Timer::Start()
{
	m_prevTime = getCurrent();
}

double Timer::Stop()
{
	return (double)(getCurrent() - m_prevTime) / m_freq;
}