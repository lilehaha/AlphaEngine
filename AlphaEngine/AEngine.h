#pragma once
#include "stdafx.h"
#include "AWindow.h"
#include "ARender.h"

namespace Alpha
{
	enum class EPlatform
	{
		Unknown,
		Windows,
		IOS,
		Android,
	};
	class AEngine : public Singleton<AEngine>
	{
	public:
		AEngine();
		~AEngine();

		bool Init();
		void Start();
		void Tick();

		void BeginFrame();
		void EndFrame();

		std::shared_ptr<AWindow> GetWindow();
		EPlatform GetCurrentPlatform();

	private:
		std::shared_ptr<AWindow> mWindow;
		std::unique_ptr<ARender> mRender;
		AGameTimer mTimer;
		std::wstring mEngineName;

		bool IsRunning;
		EPlatform mCurPlatform;

		UINT mDisplayWidth;
		UINT mDisplayHeight;
	};
}
