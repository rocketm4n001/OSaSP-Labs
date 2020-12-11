#pragma once

#include <Windows.h>

class CThread
{
	HANDLE _hThread;
public:
	// Constructor
	CThread();

	// Destructor
	~CThread();

	// Get the handle of the thread
	HANDLE GetHThread();

	// Creates a new thread
	VOID CreateNewThread(LPTHREAD_START_ROUTINE);
};
