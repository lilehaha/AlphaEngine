#include "stdafx.h"
#include "AEngine.h"
#include "WindowBuilder.h"

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
	if (!mWindow)
	{
		return false;
	}

	mAssetManager = std::make_shared<AssetManager>();
	mScene = std::make_shared<AScene>();
	mRender = std::make_unique<ARenderer>();
	mShaderManager = std::make_unique<AShaderManager>();
	mMaterialManager = std::make_unique<AMaterialManager>();

	if (!mRender->Init())
	{
		return false;
	}
	return true;
}

void AEngine::Tick()
{
	mTimer.Tick();
	mWindow->CalculateFrameStats(mTimer);
	mScene->Update();
	mRender->Render();
}

void AEngine::Destory()
{
	if (mWindow != nullptr)
	{
		mWindow = nullptr;
	}
	if (mRender != nullptr)
	{
		mRender = nullptr;
	}
	if (mScene != nullptr)
	{
		mScene = nullptr;
	}
	if (mAssetManager != nullptr)
	{
		mAssetManager == nullptr;
	}
}

std::shared_ptr<AWindow> AEngine::GetWindow()
{
	return mWindow;
}

std::shared_ptr<AScene> AEngine::GetScene()
{
	return mScene;
}

std::shared_ptr<AssetManager> AEngine::GetAssetManager()
{
	return mAssetManager;
}

float AEngine::GetTotalTime()
{
	return mTimer.TotalTime();
}

EPlatform AEngine::GetCurrentPlatform()
{
	return mCurPlatform;
}

void AEngine::Start()
{
	mTimer.Reset();
	mTimer.Start();
	IsRunning = true;
	mRender->RenderStart();
	while (IsRunning && mWindow->Run())
	{
		Tick();
	}
}
