#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <mbstring.h>
#include <tchar.h> 
#include "misc.h"

using namespace std;

#pragma comment(lib, "ComCtl32.Lib")

const wchar_t g_szClassName[] = L"MainWindow";
const wchar_t g_szWindowName[] = L"Phone Directory Sample";

#define IDC_LISTVIEW 1111

HWND hEdit;
TCHAR  ListItem[256] = L"Phone";
HWND hWndListView;


VOID InsertListViewItems(HWND hWndListView, vector<Address*> items)
{
	ListView_DeleteAllItems(hWndListView);
	LVITEM lvI;

	lvI.pszText = LPSTR_TEXTCALLBACK;
	lvI.mask = LVIF_TEXT;
	lvI.iSubItem = 0;
	
	for (int i = 0; i < items.size(); i++) 
	{
		lvI.iItem = i;

		ListView_InsertItem(hWndListView, &lvI);

		ListView_SetItemText(hWndListView, i, 0, ConvertToLPWSTR(items[i]->_szPhone));
		ListView_SetItemText(hWndListView, i, 1, ConvertToLPWSTR(items[i]->_szSurname));
		ListView_SetItemText(hWndListView, i, 2, ConvertToLPWSTR(items[i]->_szName));
		ListView_SetItemText(hWndListView, i, 3, ConvertToLPWSTR(items[i]->_szPatronymic));
		ListView_SetItemText(hWndListView, i, 4, ConvertToLPWSTR(items[i]->_szStreet));
		ListView_SetItemText(hWndListView, i, 5, ConvertToLPWSTR(IntToStr(items[i]->_iHome)));
		ListView_SetItemText(hWndListView, i, 6, ConvertToLPWSTR(IntToStr(items[i]->_iHousing)));
		ListView_SetItemText(hWndListView, i, 7, ConvertToLPWSTR(IntToStr(items[i]->_iFlat)));
	}
}

VOID InitListViewColumns(HWND hWndListView)
{
	RECT rcListView;
	LVCOLUMN lvc;

	GetClientRect(hWndListView, &rcListView);
	lvc.mask = LVCF_WIDTH | LVCF_TEXT;
	lvc.cx = 100;

	for (int i = 0; i < g_szColumnsName.size(); i++) 
	{
		lvc.pszText = const_cast<LPWSTR>(g_szColumnsName[i]);

		ListView_InsertColumn(hWndListView, i, &lvc);
	}
}

HWND CreateListView(HWND hWndParent)
{
	HWND hWndListView;
	INITCOMMONCONTROLSEX icex;
	RECT rcClient;

	// Initialize Common Controls
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	GetClientRect(hWndParent, &rcClient);

	hWndListView = CreateWindowEx(
		NULL, 
		WC_LISTVIEW, 
		L"", 
		WS_CHILD | LVS_REPORT | LVS_EDITLABELS, 
		0, 40, 
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		hWndParent, 
		(HMENU)IDC_LISTVIEW, 
		GetModuleHandle(NULL), 
		NULL
	);

	return hWndListView;
}

HMODULE hDll = LoadLibrary(L"DatabasePhoneDirectory.dll");

/* Get the address of exported functions from DLL */
ldb_t LoadDataBase = (ldb_t)GetProcAddress(hDll, "?LoadDataBase@@YA?AV?$vector@PAVAddress@@V?$allocator@PAVAddress@@@std@@@std@@XZ");
search_t SearchAddresses = (search_t)GetProcAddress(hDll, "?SearchAddresses@@YA?AV?$vector@PAVAddress@@V?$allocator@PAVAddress@@@std@@@std@@HPAUnode@@@Z");
hash_t HashCode = (hash_t)GetProcAddress(hDll, "?HashCode@@YAHV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
phone_t GetPhoneIndex = (phone_t)GetProcAddress(hDll, "?GetPhoneIndex@@YAPAUnode@@XZ");
street_t GetStreetIndex = (street_t)GetProcAddress(hDll, "?GetStreetIndex@@YAPAUnode@@XZ");
surname_t GetSurnameIndex = (surname_t)GetProcAddress(hDll, "?GetSurnameIndex@@YAPAUnode@@XZ");

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) 
	{
		std::vector<Address*> result = LoadDataBase();

		hWndListView = CreateListView(hWnd);
		InitListViewColumns(hWndListView);
		InsertListViewItems(hWndListView, result);
		ShowWindow(hWndListView, SW_SHOWDEFAULT);

		HWND hWndComboBox = CreateWindow(
			WC_COMBOBOX, TEXT(""),
			CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			220, 
			10, 
			200, 
			200, 
			hWnd, 
			NULL, 
			GetModuleHandle(NULL),
			NULL);
		ShowWindow(hWndComboBox, SW_SHOWDEFAULT);

		TCHAR tTmp[16];
		wcscpy_s(tTmp, sizeof(tTmp) / sizeof(TCHAR), (TCHAR*)TEXT("Surname"));
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tTmp);

		wcscpy_s(tTmp, sizeof(tTmp) / sizeof(TCHAR), (TCHAR*)TEXT("Street"));
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tTmp);

		wcscpy_s(tTmp, sizeof(tTmp) / sizeof(TCHAR), (TCHAR*)TEXT("Phone"));
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tTmp);

		SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

		hEdit = CreateWindow(
			L"Edit", 
			NULL, 
			WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL, 
			10, 
			10, 
			200, 
			20, 
			hWnd, 
			NULL,
			NULL, 
			0);

		ShowWindow(hEdit, SW_SHOWDEFAULT);

		HWND hwndButton = CreateWindow(
			L"BUTTON",
			L"Find",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			590, 
			10, 
			40,
			20,
			hWnd,
			(HMENU)10000,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);
		ShowWindow(hwndButton, SW_SHOWDEFAULT);
	}

	if (uMsg == WM_COMMAND) 
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
		}

		if (LOWORD(wParam) == 10000)
		{
			char str[255];
			ZeroMemory(str, sizeof(str));
			wchar_t wtext[255];
			std::mbstowcs(wtext, str, strlen(str));
			LPWSTR ptr = wtext;
			SendMessage(hEdit, WM_GETTEXT, (WPARAM)255, (LPARAM)ptr);

			char buffer[500];

			wcstombs(buffer, ptr, 500);

			ListView_DeleteAllItems(hWndListView);
			std::vector<Address*> result;

			if (lstrcmpW((LPCWSTR)ListItem,  L"Phone") == 0) 
			{
				result = SearchAddresses(HashCode(buffer), GetPhoneIndex());
			}

			if (lstrcmpW((LPCWSTR)ListItem, L"Street") == 0) 
			{
				result = SearchAddresses(HashCode(buffer), GetStreetIndex());
			}

			if (lstrcmpW((LPCWSTR)ListItem, L"Surname") == 0) 
			{
				result = SearchAddresses(HashCode(buffer), GetSurnameIndex());
			}

			InsertListViewItems(hWndListView, result);

			UpdateWindow(hWndListView);
		}
	}

	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	HWND hMainWnd;
	MSG msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_szClassName;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);

	hMainWnd = CreateWindowEx(
		NULL, 
		g_szClassName, 
		g_szWindowName, 
		dwStyle, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		800, 600, 
		NULL, 
		NULL, 
		hInstance, 
		NULL
	);

	if (!hMainWnd)
	{
		MessageBox(NULL, L"Failed to create a new window", L"Error", MB_OK);

		return -1;
	}

	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}