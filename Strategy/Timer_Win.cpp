#include "Timer.h"
#include <Windows.h>

Timer::Timer()
{
	_LARGE_INTEGER lif;
	QueryPerformanceFrequency(&lif);

	freq = (double)lif.QuadPart;

	prevTime = getCurrent();
}

__int64 Timer::getCurrent()
{
	_LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

void Timer::Start()
{
	prevTime = getCurrent();
}

double Timer::Stop()
{
	return (double)(getCurrent() - prevTime) / freq;
}