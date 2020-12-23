#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include "CCriticalSection.h"

/*typedef struct
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
} PROCESS_INFO;
*/

int wmain(int argc, wchar_t* argv[])
{
	CCriticalSection cs;

	if (argc == 1)
	{
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi;

		//ZeroMemory(&pInfo.pi, sizeof(PROCESS_INFORMATION));

		BOOL bResult = CreateProcessW(
			L"CrititicalSectionDemo.exe",
			(wchar_t*)L"Second process",
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&pi
		);

		if (!bResult)
		{
			wprintf_s(L"Failed to create a process. Error: %d\n\n",
				GetLastError());

			system("pause");

			return 0;
		}

		for (INT i = 0; i < 50; i++)
		{
			cs.WriteSharedMemory(i);

			Sleep(1000);
		}

		system("pause");
	}
	else
	{
		cs.ReadSharedMemory();

		Sleep(500);
	}

	system("pause");

	return 0;
}