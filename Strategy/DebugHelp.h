#pragma once

#ifndef NDEBUG
	#define WARNING_INFO(msg) printWarning(msg, __FUNCTION__, __FILE__, __LINE__); 
	#define ERROR_INFO(msg) printError(msg, __FUNCTION__, __FILE__, __LINE__); 
	#define WARNING(msg) printWarning(msg); 
	#define ERROR(msg) printError(msg); 
#else
	#define WARNING_INFO(msg)
	#define ERROR_INFO(msg)
	#define WARNING(msg)
	#define ERROR(msg)
#endif

void printWarning(const char* msg, const char* func, const char* path, unsigned int line);
void printError(const char* msg, const char* func, const char* path, unsigned int line);
void printWarning(const char* msg);
void printError(const char* msg);