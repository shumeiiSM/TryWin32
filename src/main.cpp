#include <windows.h>	// Windows API (Application Programming Interface) - Contains declaration of all these functions
#include <gl/GL.h>
#include <gl/GLU.h>

/* Display a triangle */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	//glIndexi(1);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(0, 1);

	//glIndexi(2);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2i(-1, -1);

	//glIndexi(3);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2i(1, -1);

	glEnd();
	glFlush();
}

/* Multiple Triangle by just adding more glVertex */
void display2()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2d(-1.0, 1.0);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2d(-1.0, -1.0);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2d(0.0, 0.0);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2d(1.0, 1.0);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2d(0.0, 0.0);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2d(1.0, -1.0);

	glEnd();

	glFlush();
}

/* Using glDrawElements to draw multiple */
void display3() 
{
	/* Must enable to use the array calls in glDrawElement, glArrayElement, or glDrawArray */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	struct Tri
	{
		float pos[2];
		float col[3];

	} triVert[6] = {
		{{-1.0,  1.0}, {1, 0, 0}},   // 0
		{{-1.0, -1.0}, {0, 1, 0}},   // 1
		{{ 0.0,  0.0}, {0, 0, 1}},   // 2
		{{ 1.0,  1.0}, {1, 0, 0}},   // 3
		{{ 0.0,  0.0}, {1, 0, 0}},   // 4
		{{ 1.0, -1.0}, {0, 0, 1}}    // 5
	};

	GLushort triIndices[5] = { 0, 4, 3, 1, 2 }; // sequence is important as it will display based on the array order u put
	glVertexPointer(2, GL_FLOAT, sizeof(Tri), triVert[0].pos);
	glColorPointer(3, GL_FLOAT, sizeof(Tri), triVert[0].col);
	
	glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_SHORT, triIndices);

	glFlush(); // function forces execution of OpenGL functions in finite time.
} 

/* Window-Procedure Function: Process messages sent to a window */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND CreateOpenGLWindow(BYTE type, DWORD flags)
{
	static HINSTANCE hInstance = 0;
	HDC hdc; // Handle Device Context

	PIXELFORMATDESCRIPTOR pfd;
	int pf;

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
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		//wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		/* Register WNDCLASS with Window so that it knows about your window & how to send messages to it */
		//RegisterClass(&wcex);

		if (!RegisterClassW(&wcex))
		{
			MessageBox(NULL, L"Call to RegisterClassW failed!", NULL, MB_OK);
			//return 1;
			exit(1);
		}
	}

	/* Create the window */
	HWND hwnd = CreateWindowEx(
		0,							   // Extended Window Style
		szWindowClass,				   // System Class Name
		szTitle,					   // Window Name in title bar
		WS_OVERLAPPEDWINDOW,		   // Window Style
		CW_USEDEFAULT, CW_USEDEFAULT,  // X, Y  --> int horizontal / vertical position of window
		500, 500,					   // Width, Height --> in device units of the window
		NULL,						   // Parent Window Handle
		NULL,						   // Menu Handle
		hInstance,					   // Instance Handle to associate with the window
		NULL);						   // Additional Application Data

	// Check if successfully created a new window
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Call to CreateWindowEx failed!", NULL, MB_OK);
		//return 1;
		exit(1);
	}

	// Retrieves a handle to a device context (DC) for the client area of a specified window or for the entire screen
	hdc = GetDC(hwnd);

	//memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
	pfd.iPixelType = type;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat(hdc, &pfd);
	if (pf == 0) {
		MessageBox(NULL, L"ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.", L"Error", MB_OK);
		return 0;
	}

	if (SetPixelFormat(hdc, pf, &pfd) == FALSE) {
		MessageBox(NULL, L"SetPixelFormat() failed:  "
			"Cannot set format specified.", L"Error", MB_OK);
		return 0;
	}

	DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	ReleaseDC(hwnd, hdc);

	return hwnd;
}

// main function as the starting point
/* WinMain - Application Entry Point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HDC hdc;				/* device context */
	HGLRC hrc;				/* opengl context */
	HWND  hwnd;				/* window */
	MSG   msg;				/* message */

	hwnd = CreateOpenGLWindow(PFD_TYPE_RGBA, 0);
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Call to CreateOpenGLWindow failed!", NULL, MB_OK);
		exit(1);
	}

	hdc = GetDC(hwnd);
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	ShowWindow(hwnd, nShowCmd);
	//UpdateWindow(hwnd);


	// GUI Application - respond to event (communicate with Window & pass message)
	//MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hwnd, hdc);
	wglDeleteContext(hrc);
	DestroyWindow(hwnd);

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
		//display();
		display2();

		//float positions[] = {
		//	-0.5f,  0.5f,
		//	 0.5f, -0.5f,
		//	-0.5f, -0.5f
		//};

		//glDrawBuffer(GL_FRONT_AND_BACK);

		display3();

		PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hwnd, &ps);
		//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}