#pragma once

class Timer
{
private:
	double freq;
	__int64 prevTime;

	__int64 getCurrent();
public:
	Timer();

	void Start();
	double Stop();
};