#include "stdafx.h"
#include "AWindow.h"

AWindow* AWindow::InitWindow(UINT Width, UINT Height, const std::string& Name)
{
	AWindow* mWindow = nullptr;
#ifdef PlatformWindows
	mWindow = new AWindowWin32(Width, Height, Name, nullptr);
#elif defined(PlatformIOS)
	mWindow = new AWindowIOS();
#elif defined(PlatformAndroid)
	mWindow = new AWindowAndroid();
#endif
	return mWindow;
}

void AWindow::DestoryWindow(AWindow* Window)
{
	ReleasePtr(Window);
}

AWindow::AWindow(UINT Width, UINT Height) : mWidth(Width), mHeight(Height)
{

}

AWindow::~AWindow()
{

}

#ifdef PlatformWindows
static AWindowWin32* mWndWin32 = nullptr;
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!mWndWin32)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif

AWindowWin32::AWindowWin32(UINT Width, UINT Height, const std::string& Name, void* HWnd) : AWindow(Width, Height), mHWnd((HWND)HWnd)
{
	InstantiateWindow(Name);
	mWndWin32 = this;
}

AWindowWin32::~AWindowWin32()
{
	mWndWin32 = nullptr;
}

bool AWindowWin32::Run()
{
	MSG msg = { 0 };
	bool isQuit = false;
	while (PeekMessage(&msg,0,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			isQuit = true;
		}
	}
	return !isQuit;
}

void AWindowWin32::InstantiateWindow(const std::string& Name)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = LoadIcon(0, IDC_ARROW);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed !", 0, 0);
	}
	RECT R = { 0,0,mWidth,mHeight };
	AdjustWindowRect(&R, WS_OVERLAPPED, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mHWnd = CreateWindow(L"MainWnd", L"AlphaEngine",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, GetModuleHandle(0), 0);
	if (!mHWnd)
	{
		MessageBox(0, L"CreateWindow Failed", 0, 0);
	}
	else
	{
		ShowWindow(mHWnd, SW_SHOW);
		UpdateWindow(mHWnd);
		SetActiveWindow(mHWnd);
		SetForegroundWindow(mHWnd);
	}
}
