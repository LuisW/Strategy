#pragma once

class Console
{
private:
	static Console* sm_pInstance;

	bool m_active;

public:
	Console();
	void init();
	void deinit();
};