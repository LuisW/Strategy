#include "DebugHelp.h"
#include <iostream>

void filename(const char* path, char* out, unsigned int len)
{
	const char* str = path + strlen(path);
	for (; *str != '/' && *str != '\\' && str != path; str--);
	strncpy(out, str + 1, len);
	out[len - 1] = 0;
}

void printWarning(const char* msg, const char* func, const char* path, unsigned int line)
{
	char file[256];
	filename(path, file, 256);

	std::cout << "Warning in " << func << "! (" << file << ", " << line << ")" << std::endl;
	std::cout << msg << std::endl;
}

void printError(const char* msg, const char* func, const char* path, unsigned int line)
{
	char file[256];
	filename(path, file, 256);

	std::cout << "Error in " << func << "! (" << file << ", " << line << ")" << std::endl;
	std::cout << msg << std::endl;
	system("pause");
	exit(-1);
}

void printWarning(const char* msg)
{
	std::cout << "Warning: " << msg << std::endl;
}

void printError(const char* msg)
{
	std::cout << "Error: " << msg << std::endl;
	system("pause");
	exit(-1);
}