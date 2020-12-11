#pragma once

#include <Windows.h>
#include "CThread.h"
#include "CThreadPoolException.h"
#include "CLogger.h"

class CThreadPool
{
	// Pointer to array of threads in TP
	CThread* _threads;

	// Number of threads in TP
	INT _nThreads;

	// Max number of threads
	INT _iMaxThreads;

	// Gets the index of free thread
	INT GetFreeThreadIndex();

	// Checks whether the thread is available
	BOOL IsThreadAvailable(CThread*);

	// Checks whether the thread is active
	BOOL IsThreadActive(CThread*, DWORD dwStatus);

	// Initialization of TP
	VOID Init(INT, INT);

	CLogger* _logger;

public:
	// Constructor
	CThreadPool(INT, INT);
	CThreadPool(INT);

	// Destructor
	~CThreadPool();

	// Adds a new task by creating a new thread
	VOID AddTaskTP(LPTHREAD_START_ROUTINE);

	// Increments the number of threads
	VOID AddThreadTP();

	// Decrements the number of threads 
	VOID DeleteThreadTP();
};