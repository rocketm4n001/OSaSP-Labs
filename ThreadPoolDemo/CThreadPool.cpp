#include "CThreadPool.h"

#define INIT_NUM_THREADS 5

CThreadPool::CThreadPool(INT nThreads, INT iMaxThreads)
{
	Init(nThreads, iMaxThreads);
}

CThreadPool::CThreadPool(INT iMaxThreads)
{
	Init(INIT_NUM_THREADS, iMaxThreads);
}

void CThreadPool::Init(INT nThreads, INT iMaxThreads)
{
	this->_iMaxThreads = iMaxThreads;

	if (nThreads <= iMaxThreads)
	{
		this->_threads = new CThread[iMaxThreads];

		this->_nThreads = nThreads;
	}
	else
	{
		throw CThreadPoolException((char*)"The maximum number of threads cannot be less than the initialized.\n");
	}

	_logger = new CLogger((wchar_t*)L"logs.log");

	_logger->MakeLogInitTP(nThreads, iMaxThreads);
}

BOOL CThreadPool::IsThreadAvailable(CThread* t)
{
	DWORD dwStatus;

	GetExitCodeThread(t->GetHThread(), &dwStatus);

	return (IsThreadActive(t, dwStatus) ? TRUE : FALSE);
}

BOOL CThreadPool::IsThreadActive(CThread* thread, DWORD dwStatus)
{
	if (dwStatus == STILL_ACTIVE)
	{
		return FALSE;
	}

	TerminateThread(thread->GetHThread(), 0);

	return TRUE;
}

INT CThreadPool::GetFreeThreadIndex()
{
	for (INT i = 0; i < _nThreads; i++)
	{
		if (IsThreadAvailable(&_threads[i]))
		{
			return i;
		}
	}

	return -1;
}

VOID CThreadPool::AddThreadTP()
{
	if (_nThreads < _iMaxThreads)
	{
		_nThreads++;
	}
	else
	{
		_logger->MakeLogTooManyThreadsTP();
	}
}

VOID CThreadPool::AddTaskTP(LPTHREAD_START_ROUTINE lpStartAddress)
{
	INT i = GetFreeThreadIndex();

	if (i != -1)
	{
		this->_threads[i].CreateNewThread(lpStartAddress);

		_logger->MakeLogAddedTask((int)_threads[i].GetHThread());
	}
	else
	{
		_logger->MakeLogNoThreadsTP();
	}
}

VOID CThreadPool::DeleteThreadTP()
{
	if (_nThreads > 1)
	{
		_nThreads--;
	}
	else
	{
		_logger->MakeLogTooFewThreadsTP();
	}
}

CThreadPool::~CThreadPool()
{
	_logger->MakeLogEndTP();

	_logger->~CLogger();
}
