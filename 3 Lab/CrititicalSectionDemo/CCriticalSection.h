#pragma once

#include <Windows.h>
#include <iostream>

class CCriticalSection
{
	HANDLE _hMap = NULL;
	HANDLE _hMutex = NULL;
	PWSTR _pwBuffer = NULL;

	VOID ProjectionFileSharedMemory();

public:
	// Constructor
	CCriticalSection();

	// Destructor
	~CCriticalSection();

	VOID WriteSharedMemory(INT);
	VOID ReadSharedMemory();
};
