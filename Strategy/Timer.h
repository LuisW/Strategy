#pragma once

class Timer
{
private:
	double m_freq;
	__int64 m_prevTime;

	__int64 getCurrent();
public:
	Timer();

	void Start();
	double Stop();
};