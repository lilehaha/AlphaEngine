#pragma once
#include "stdafx.h"
#include "AWindow.h"
#include "ARenderer.h"
#include "AScene.h"
#include "AssetManager.h"


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
	void Destory();

	std::shared_ptr<AWindow> GetWindow();
	std::shared_ptr<AScene> GetScene();
	std::shared_ptr<AssetManager> GetAssetManager();
	float GetTotalTime();

	EPlatform GetCurrentPlatform();

private:
	std::shared_ptr<AWindow> mWindow;
	std::unique_ptr<ARenderer> mRender;
	std::shared_ptr<AScene> mScene;
	std::shared_ptr<AssetManager> mAssetManager;
	AGameTimer mTimer;
	std::wstring mEngineName;

	bool IsRunning;
	EPlatform mCurPlatform;

	UINT mDisplayWidth;
	UINT mDisplayHeight;
};

