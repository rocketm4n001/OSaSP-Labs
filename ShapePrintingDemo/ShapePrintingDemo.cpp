#include <Windows.h>

LRESULT CALLBACK WndProc(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex;

	const wchar_t szClassName[] = L"CustomClass";

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassName;
	wcex.hIconSm = wcex.hIcon;

	RegisterClassEx(&wcex);

	DWORD dwWndStyle = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	HWND hWnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, szClassName, L"Shape Printing Demo", dwWndStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBoxW(NULL, L"Failed to create a new window", L"Error", MB_OK);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message,
	_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	static HDC hDC;
	static HFONT hFont;
	static LOGFONTW lfw;
	static HPEN hrPen, hgPen, hbPen, hyPen, hpPen;
	static HBRUSH hrBrush, hgBrush, hbBrush, hyBrush, hpBrush;
	static COLORREF cPrev;

	switch (message)
	{
	case WM_CREATE:
		// Get device context of the window
		hDC = GetDC(hWnd);

		// Create a new font
		ZeroMemory(&lfw, sizeof(lfw));
		lfw.lfWeight = FW_BOLD;
		lfw.lfItalic = TRUE;
		wcscpy_s(lfw.lfFaceName, LF_FACESIZE, L"Tahoma");

		hFont = CreateFontIndirectW(&lfw);

		// Create a pen
		hrPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		hgPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		hbPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
		hyPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
		hpPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 255));

		hrBrush = CreateSolidBrush(RGB(255, 0, 0));
		hgBrush = CreateSolidBrush(RGB(0, 255, 0));
		hbBrush = CreateSolidBrush(RGB(0, 0, 255));
		hyBrush = CreateSolidBrush(RGB(255, 255, 0));
		hpBrush = CreateSolidBrush(RGB(255, 0, 255));

		break;
	case WM_PAINT:
	{
		// Drawing a line
		SelectObject(hDC, hFont);
		TextOutW(hDC, 50, 25, L"Line", 4);

		SelectObject(hDC, hrPen);
		MoveToEx(hDC, 30, 100, NULL);
		LineTo(hDC, 125, 75);

		// Drawing a polyline
		TextOutW(hDC, 300, 25, L"Polyline", 8);
		SelectObject(hDC, hgPen);

		POINT ptsPolyline[4] = { {300, 75}, {350, 100}, {400, 75}, {450, 100} };
		Polyline(hDC, ptsPolyline, 4);

		// Drawing a rectangle
		TextOutW(hDC, 600, 25, L"Rectangle", 9);

		SelectObject(hDC, hyPen);
		SelectObject(hDC, hbBrush);
		Rectangle(hDC, 580, 65, 730, 115);
		
		// Drawing a polygon
		TextOutW(hDC, 50, 200, L"Polygon", 7);
		
		SelectObject(hDC, hgPen);
		SelectObject(hDC, hpBrush);
		POINT ptsPolygon[4] = { {50, 270}, {80, 350}, {150, 420}, {120, 300} };

		Polygon(hDC, ptsPolygon, 4);

		// Drawing an ellipse
		TextOutW(hDC, 300, 200, L"Ellipse", 7);

		SelectObject(hDC, hbPen);
		SelectObject(hDC, hrBrush);

		Ellipse(hDC, 270, 300, 450, 400);

		// Drawing a text label 
		TextOutW(hDC, 600, 200, L"Text Label", 10);

		cPrev = SetTextColor(hDC, RGB(0, 255, 255));
		TextOutW(hDC, 580, 350, L"Always smile:)", 14);

		SetTextColor(hDC, cPrev);

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	return 0;
}