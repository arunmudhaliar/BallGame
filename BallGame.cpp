// BallGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BallGame.h"

#include "source/engine/core/Timer.h"
#include "source/engine/sceneManager/SceneManager.h"

#include "source/scenes/mainmenu.h"
#include "source/scenes/gameScene.h"

#define MAX_LOADSTRING 100

#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768

SceneManager m_cSceneManager;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BALLGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BALLGAME));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BALLGAME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_BALLGAME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		UINT_PTR timerID = SetTimer(hWnd, 1, 16, (TIMERPROC)NULL);
		timerID=timerID;

		setResourcePath("");
		Timer::init();
		m_cSceneManager.init();
		Scene::getCommonData()->init(NULL, false, rendererBase::gl_fixed_pipeline, hWnd);
		Scene::getCommonData()->setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		m_cSceneManager.resize(SCREEN_WIDTH, SCREEN_HEIGHT);
		m_cSceneManager.appendScene(new mainMenu());
		m_cSceneManager.appendScene(new gameScene());

		m_cSceneManager.setActiveScene(m_cSceneManager.getSceneByID(0));
	}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
		{
			Scene::getCommonData()->setScreenSize(LOWORD(lParam), HIWORD(lParam));
			m_cSceneManager.resize(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	case WM_TIMER:
		{
			Timer::update();
			m_cSceneManager.update(Timer::getDTinMilliSec());

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_cSceneManager.render();

			Scene::getCommonData()->getRendererPtr()->swapGLBuffer();
		}
		break;
	//case WM_PAINT:
	//	{
	//		//Timer::update();
	//		//m_cSceneManager.update(Timer::getDTinMilliSec());

	//		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//		//m_cSceneManager.render();

	//		//Scene::getCommonData()->getRendererPtr()->swapGLBuffer();
	//	}
	//	break;
	case WM_KEYDOWN:
		{
			if(LOWORD(wParam)==VK_ESCAPE && m_cSceneManager.getActiveScene()->getSceneID()==0)
			{
				PostQuitMessage(0);
			}
			else
			{
				m_cSceneManager.keyDown(LOWORD(wParam));
			}
		}
		break;
	case WM_KEYUP:
		{
			m_cSceneManager.keyUp(LOWORD(wParam));
		}
		break;

	case WM_MOUSEMOVE:
	{
		int nFlags = LOWORD(wParam);
		int xx = LOWORD(lParam);
		int yy = HIWORD(lParam);
		m_cSceneManager.touchMoved(xx, yy, &nFlags);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		int nFlags = LOWORD(wParam);
		int xx = LOWORD(lParam);
		int yy = HIWORD(lParam);
		m_cSceneManager.touchBegin(xx, yy, &nFlags);
	}
	break;
	case WM_LBUTTONUP:
	{
		int nFlags = LOWORD(wParam);
		int xx = LOWORD(lParam);
		int yy = HIWORD(lParam);
		m_cSceneManager.touchEnd(xx, yy, &nFlags);
	}
	break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
