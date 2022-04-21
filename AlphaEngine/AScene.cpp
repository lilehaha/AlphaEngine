#include "stdafx.h"
#include "AScene.h"

AScene::AScene()
{
	mCamera = std::make_shared<ACamera>();
	mCameraController = std::make_unique<ACameraController>();
	mCameraController->SetCameraObj(mCamera);
}

AScene::~AScene()
{

}

void AScene::Update()
{
	mCameraController->Update();
}

std::shared_ptr<ACamera> AScene::GetCamera()
{
	return mCamera;
}
