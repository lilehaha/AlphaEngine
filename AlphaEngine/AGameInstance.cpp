#include "stdafx.h"
#include "AGameInstance.h"


void AGameInstance::Init(HINSTANCE hInstance)
{
	mEngine = std::make_unique<Alpha::AEngine>();
	mGameLogic = std::make_unique<AGameLogic>();
	if (mEngine->Init())
	{
		mGameLogic->Init();
	}
}

void AGameInstance::UpDate()
{
	mEngine->Start();
}

void AGameInstance::Destroy()
{
	mEngine->Destory();
	mGameLogic->Destory();
}
