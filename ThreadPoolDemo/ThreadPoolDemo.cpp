#include <iostream>
#include <Windows.h>
#include "CThreadPool.h"

VOID Proc1()
{
	wprintf_s(L"The beginning of Proc1\n");

	for (int i = 0; i < 10; i++)
	{
		wprintf(L"The square of %d is %d\n", i, i * i);
		Sleep(50);
	}

	wprintf_s(L"The end of Proc1\n\n");
}

VOID Proc2()
{
	wprintf_s(L"The beginning of Proc2\n");

	const int n = 10;

	int iArr[n] = { 5, 9, 3, 4, 8, 1, 4, 5, 3, 1 };
	int iTemp;

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (iArr[j] > iArr[j + 1])
			{
				iTemp = iArr[j];
				iArr[j] = iArr[j + 1];
				iArr[j + 1] = iTemp;
			}
		}
	}

	for (int i = 0; i < n; i++)
		wprintf_s(L"%d ", iArr[i]);

	wprintf_s(L"\nThe end of Proc2\n\n");
}

void DelayProc1()
{
	wprintf_s(L"The beginning of DelayProc1\n");

	Sleep(2500);

	wprintf_s(L"The end of DelayProc2\n\n");
}

VOID DelayProc2()
{
	wprintf_s(L"The beginning of DelayProc2\n");

	Sleep(1000);

	wprintf_s(L"The beginning of DelayProc2\n\n");
}

int wmain()
{
	try
	{
		// Created 5 threads after initialization of TP (8 max)
		CThreadPool* tp = new CThreadPool(5, 8);

		// Add first task for 1 thread
		tp->AddTaskTP((LPTHREAD_START_ROUTINE)Proc2);
		// Add second task for 2 thread 
		tp->AddTaskTP((LPTHREAD_START_ROUTINE)Proc2);
		// Add third task for 3 thread
		tp->AddTaskTP((LPTHREAD_START_ROUTINE)Proc1);
		// Add fourth task for 4 thread
		tp->AddTaskTP((LPTHREAD_START_ROUTINE)DelayProc2);
		// Add fifth task for 5 thread
		tp->AddTaskTP((LPTHREAD_START_ROUTINE)DelayProc1);
		// No available threads in TP after adding a new task
		tp->AddTaskTP((LPTHREAD_START_ROUTINE)Proc2);

		tp->~CThreadPool();
	}
	catch (const CThreadPoolException& e)
	{
		printf(e.what());
	}

	cin.get();

	return 0;
}