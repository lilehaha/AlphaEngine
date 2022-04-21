#include "stdafx.h"
#include "AEngine.h"
#include "WindowBuilder.h"

namespace Alpha
{
	AEngine::AEngine()
	{
#ifdef PlatformWindows
		mCurPlatform = EPlatform::Windows;
		mDisplayWidth = 1920;
		mDisplayHeight = 1080;
		mEngineName = L"AlphaEngine";
#elif defined(PlatformIOS)
		PCurrentPlatform = EPlatform::IOS;
#elif defined(PlatformAndroid)
		PCurrentPlatform = EPlatform::Android;
#endif
	}

	AEngine::~AEngine()
	{

	}

	bool AEngine::Init()
	{
		WindowInfo mWindowInfo;
		mWindowInfo.Width = mDisplayWidth;
		mWindowInfo.Height = mDisplayHeight;
		mWindowInfo.Name = mEngineName;

		std::unique_ptr<WindowBuilder> mWindowBuilder = std::make_unique<WindowBuilder>();
		mWindow = mWindowBuilder->CreateMainWindow();
		mWindow->InitWindow(mWindowInfo);

		mScene = std::make_shared<AScene>();
		if (!mWindow)
		{
			return false;
		}
		mRender = std::make_unique<ARenderer>();
		if (!mRender->Init())
		{
			return false;
		}
		return true;
	}

	void AEngine::Tick()
	{
		mTimer.Tick();
		mRender->Render();
		mScene->Update();
		mWindow->CalculateFrameStats(mTimer);
	}

	std::shared_ptr<AWindow> AEngine::GetWindow()
	{
		return mWindow;
	}

	std::shared_ptr<AScene> AEngine::GetScene()
	{
		return mScene;
	}

	Alpha::EPlatform AEngine::GetCurrentPlatform()
	{
		return mCurPlatform;
	}

	void Alpha::AEngine::Start()
	{
		mTimer.Reset();
		mTimer.Start();
		IsRunning = true;
		while (IsRunning && mWindow->Run())
		{
			Tick();
		}
	}
}
