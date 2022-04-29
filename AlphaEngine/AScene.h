#pragma once

#include "ACamera.h"
#include "ACameraController.h"
#include "Actor.h"

class AScene
{
public:
	AScene();
	~AScene();
	void Update();
	std::shared_ptr<ACamera> GetCamera();

	bool LoadScene(const std::string& Path);
	std::unordered_map<std::string, ActorInfo*>& GetAllActor();
public:
	std::shared_ptr<ACamera> mCamera;
	std::unique_ptr<ACameraController> mCameraController;


	std::string mSceneInfo;
	std::unordered_map<std::string, ActorInfo*> mActors;
};

