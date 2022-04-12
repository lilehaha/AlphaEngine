#pragma once
#include "stdafx.h"
#include "AWindow.h"

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

		void Init();

	private:
		std::shared_ptr<AWindow> mAWindow;
		EPlatform mCurPlatform;
	};
}
