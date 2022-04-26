#include "stdafx.h"
#include "AWindowWin32.h"
#include "AEngine.h"
#include "InputBuilder.h"

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
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		return 0;
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		AEngine::GetSingleton().GetWindow()->GetInput()->OnMouseDown(KeyCode::Mouse2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		ReleaseCapture();
		AEngine::GetSingleton().GetWindow()->GetInput()->OnMouseUp(KeyCode::Mouse2);
		return 0;
	case WM_MOUSEMOVE:
		AEngine::GetSingleton().GetWindow()->GetInput()->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif

AWindowWin32::AWindowWin32()
{
	std::unique_ptr<InputBuilder> mInputBuilder = std::make_unique<InputBuilder>();
	mInput = mInputBuilder->CreateInput();
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
	mInput->Update();
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
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


HWND AWindowWin32::GetHWND()
{
	return mHWnd;
}

bool AWindowWin32::InitWindow(const WindowInfo& mWindowInfo)
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
	mWidth = mWindowInfo.Width;
	mHeight = mWindowInfo.Height;
	RECT R = { 0,0,mWidth,mHeight };
	AdjustWindowRect(&R, WS_OVERLAPPED, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mName = mWindowInfo.Name;
	mHWnd = CreateWindow(L"MainWnd", mName.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, GetModuleHandle(0), 0);
	if (!mHWnd)
	{
		MessageBox(0, L"CreateWindow Failed", 0, 0);
		return false;
	}
	else
	{
		ShowWindow(mHWnd, SW_SHOW);
		UpdateWindow(mHWnd);
		SetActiveWindow(mHWnd);
		SetForegroundWindow(mHWnd);

		return true;
	}
}

void AWindowWin32::CalculateFrameStats(const AGameTimer& gt)
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;
	//DEBUG
	/*std::wstring text = std::to_wstring(gt.TotalTime());
	std::wstring windowText = text;
	SetWindowText(mHWnd, windowText.c_str());*/

	if ((gt.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);

		std::wstring windowText = mName + L"    fps:	   " + fpsStr + L"    " + L"mspf:   " + mspfStr;
		SetWindowText(mHWnd, windowText.c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
