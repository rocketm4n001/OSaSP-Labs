#include "CCriticalSection.h"
#include <cstdio>

CCriticalSection::CCriticalSection()
{
	_hMap = CreateFileMappingW(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		128,
		L"File Mapping Object" 
	);

	if (_hMap == NULL)
	{
		wprintf_s(L"Failed to create file mapping object. Error: %d",
			GetLastError());

		return;
	}

	_hMutex = CreateMutex(
		NULL,
		FALSE,
		L"MyMutex"
	);
}

CCriticalSection::~CCriticalSection() {}

VOID CCriticalSection::WriteSharedMemory(INT iNum)
{
	WaitForSingleObject(_hMutex, INFINITE);

	wchar_t szMessage[10];
	wsprintf(szMessage, L"%d", iNum);

	/*this->_pwBuffer = (LPWSTR)MapViewOfFile(
		_hMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		10
	);*/
	ProjectionFileSharedMemory();

	if (this->_pwBuffer == NULL)
	{
		wprintf_s(L"Failed to get the starting address of mapped view\n");

		system("pause");

		return;
	}

	memcpy((PVOID)_pwBuffer, szMessage, (10 * sizeof(wchar_t)));

	wprintf_s(L"First process write: %d\n", iNum);

	ReleaseMutex(_hMutex);
}

VOID CCriticalSection::ReadSharedMemory()
{
	WaitForSingleObject(_hMutex, INFINITE);

	/*this->_pwBuffer = (LPWSTR)MapViewOfFile(
		_hMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		10
	);*/
	ProjectionFileSharedMemory();

	wprintf_s(L"Second process read: %s", _pwBuffer);

	ReleaseMutex(_hMutex);
}

VOID CCriticalSection::ProjectionFileSharedMemory()
{
	_pwBuffer = (LPWSTR)MapViewOfFile(
		_hMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		10
	);
}