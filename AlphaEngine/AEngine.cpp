#include "stdafx.h"
#include "AEngine.h"

namespace Alpha
{
	AEngine::AEngine()
	{
#ifdef PlatformWindows
		mCurPlatform = EPlatform::Windows;
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
		mWindow = AWindow::InitWindow(1080, 720, "AlphaEngine");
		if (!mWindow)
		{
			return false;
		}
		mRender = std::make_unique<ARender>();
		if (!mRender->Init())
		{
			return false;
		}
		return true;
	}

	void AEngine::Tick()
	{
		mRender->Render();

		/*	BeginFrame();
			EndFrame();*/
	}

	void AEngine::BeginFrame()
	{
		
	}

	void AEngine::EndFrame()
	{

	}

	AWindow* AEngine::GetWindow()
	{
		return mWindow;
	}

	void Alpha::AEngine::Start()
	{
		IsRunning = true;
		while (IsRunning && mWindow->Run())
		{
			Tick();
		}
	}
}
