#include "stdafx.h"
#include "AEngine.h"

namespace Alpha
{
	AEngine::AEngine()
	{

	}

	AEngine::~AEngine()
	{

	}

	void Alpha::AEngine::Init()
	{
#ifdef PlatformWindows
		mCurPlatform = EPlatform::Windows;
#elif defined(PlatformIOS)
		PCurrentPlatform = EPlatform::IOS;
#elif defined(PlatformAndroid)
		PCurrentPlatform = EPlatform::Android;
#endif
		AWindow* mAWindow = AWindow::InitWindow(1920, 1080, "AlphaEngine");
		
	}

}
