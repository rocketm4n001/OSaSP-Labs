#include "CThread.h"

CThread::CThread()
{
	_hThread = NULL;
}

HANDLE CThread::GetHThread()
{
	return _hThread;
}

void CThread::CreateNewThread(LPTHREAD_START_ROUTINE lpStartAddress)
{
	_hThread = CreateThread(NULL, 0, lpStartAddress, NULL, 0, NULL);
}

CThread::~CThread()
{
	delete _hThread;
}