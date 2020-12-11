#include "CLogger.h"

CLogger::CLogger(wchar_t* szFileName)
{
	_wfopen_s(&_fpFile, szFileName, L"a");
}

CLogger::~CLogger()
{
	fclose(_fpFile);
}

void CLogger::MakeLogInitTP(INT nThreads, INT iMaxThreads)
{
	this->MakeTimeStamp();

	fwprintf(_fpFile, L"Created (%d) threads. Max number of threads: (%d)\n", nThreads, iMaxThreads);
}

void CLogger::MakeLogAddedTask(INT hThread)
{
	this->MakeTimeStamp();

	fwprintf(_fpFile, L"New task. Thread ID: (%d)\n", hThread);
}

void CLogger::MakeLogNoThreadsTP()
{
	this->MakeTimeStamp();

	fwprintf(_fpFile, L"No available threads in TP.\n");
}

void CLogger::MakeLogTooManyThreadsTP()
{
	this->MakeTimeStamp();

	fwprintf(_fpFile, L"Too many threads in TP.\n");
}

void CLogger::MakeLogTooFewThreadsTP()
{
	this->MakeTimeStamp();

	fwprintf(_fpFile, L"Not enough threads in TP.\n");
}

void CLogger::MakeLogEndTP()
{
	this->MakeTimeStamp();

	fwprintf(_fpFile, L"End of work of TP.\n\n");
}

void CLogger::MakeTimeStamp()
{
	GetLocalTime(&_stTime);

	fwprintf(_fpFile, L"[%02d:%02d:%02d]\t", _stTime.wHour, _stTime.wMinute, _stTime.wSecond);
}

