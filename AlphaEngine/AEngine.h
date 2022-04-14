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

		AWindow* GetWindow();
	private:
		AWindow* mWindow;
		std::unique_ptr<ARender> mRender;
	
		bool IsRunning;
		EPlatform mCurPlatform;
	};
}
