#pragma once
#include "stdafx.h"
#include "ACamera.h"
#include "ACameraController.h"

class AScene
{
public:
	AScene();
	~AScene();
	void Update();
	std::shared_ptr<ACamera> GetCamera();
public:
	std::shared_ptr<ACamera> mCamera;
	std::unique_ptr<ACameraController> mCameraController;
};

