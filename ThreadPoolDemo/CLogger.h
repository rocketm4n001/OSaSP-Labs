#pragma once

#pragma region Includes & Directives

#include <Windows.h>
#include <process.h>
#include <cstdio>

#pragma endregion

#pragma region CLogger

class CLogger
{
	SYSTEMTIME _stTime;
	FILE* _fpFile;
	wchar_t* _szFileName;

	VOID MakeTimeStamp();

public:
	// Contructor
	CLogger(wchar_t*);

	// Destructor
	~CLogger();

	// Make log after initialization of TP 
	VOID MakeLogInitTP(INT, INT);

	// Make log after added a task
	VOID MakeLogAddedTask(INT);

	// Make log when there are no threads in TP
	VOID MakeLogNoThreadsTP();

	// Make log when the number of working threads more then max number in TP
	VOID MakeLogTooManyThreadsTP();

	// Make log when the leak of threads in TP
	VOID MakeLogTooFewThreadsTP();

	// Make log when the TP completes the work
	VOID MakeLogEndTP();
};

#pragma endregion