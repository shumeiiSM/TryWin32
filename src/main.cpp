#include <windows.h>	// Windows API (Application Programming Interface) - Contains declaration of all these functions

/* Window-Procedure Function: Process messages sent to a window */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// main function as the starting point
/* WinMain - Application Entry Point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	hInstance = 0;

	/* Contain info about the window */
	WNDCLASS wcex;

	const wchar_t szWindowClass[] = L"Win32 Class";
	const wchar_t szTitle[] = L"Win32 Window";

	if (!hInstance) {
		//hInstance = GetModuleHandle(NULL);

		//wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		//wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		/* Register WNDCLASS with Window so that it knows about your window & how to send messages to it */
		//RegisterClass(&wcex);

		if (!RegisterClassW(&wcex))
		{
			MessageBox(NULL, L"Call to RegisterClassW failed!", NULL, MB_OK);
			return 1;
		}
	}

	/* Create the window */
	HWND hwnd = CreateWindowEx(
		0,							   // Extended Window Style
		szWindowClass,				   // System Class Name
		szTitle,					   // Window Name in title bar
		WS_OVERLAPPEDWINDOW,		   // Window Style
		CW_USEDEFAULT, CW_USEDEFAULT,  // X, Y  --> int horizontal / vertical position of window
		1000, 500,					   // Width, Height --> in device units of the window
		NULL,						   // Parent Window Handle
		NULL,						   // Menu Handle
		hInstance,					   // Instance Handle to associate with the window
		NULL);						   // Additional Application Data

	// Check if successfully created a new window
	if (hwnd == NULL)
	{
		return 1;
		//exit(1);
	}

	ShowWindow(hwnd, nShowCmd);
	//UpdateWindow(hwnd);

	// GUI Application - respond to event (communicate with Window & pass message)
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Closing the window
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// Draw stuff on the window
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}