#include "stdafx.h"
#include "AScene.h"
#include "set"
#include "AEngine.h"

AScene::AScene()
{
	mCamera = std::make_shared<ACamera>();
	mCameraController = std::make_unique<ACameraController>();
	mCameraController->SetCameraObj(mCamera);
}

AScene::~AScene()
{
	for (auto &&actor : mActors)
	{
		ReleasePtr(actor.second);
	}
}

void AScene::Update()
{
	mCameraController->Update();
}

std::shared_ptr<ACamera> AScene::GetCamera()
{
	return mCamera;
}

bool AScene::LoadScene(const std::string& Path)
{
	mActors.clear();
	std::set<std::string> meshName;

	std::ifstream fin(Path, std::ios::binary);
	if (fin.is_open())
	{
		int len = 0;
		fin.read((char*)&len, sizeof(int32_t));
		mSceneInfo.resize(len);
		fin.read((char*)mSceneInfo.data(), sizeof(char) * len);

		int actorNum = 0;
		fin.read((char*)&actorNum, sizeof(int32_t));
		//actors

		for (size_t i = 0; i < actorNum; i++)
		{
			ActorInfo* actorInfo = new ActorInfo;
			fin.read((char*)&len, sizeof(int32_t));
			actorInfo->Name.resize(len);
			fin.read((char*)actorInfo->Name.data(), sizeof(char) * len);
			//staticMeshNames
			int meshNameNum = 0;
			fin.read((char*)&meshNameNum, sizeof(int32_t));
			actorInfo->StaticMeshNames.resize(meshNameNum);
			for (int j = 0; j < meshNameNum; j++)
			{
				fin.read((char*)&len, sizeof(int32_t));
				fin.read((char*)actorInfo->StaticMeshNames[j].data(), sizeof(char) * len);
			}
			//transforms
			fin.read((char*)&len, sizeof(int32_t));
			actorInfo->Transforms.resize(len);
			fin.read((char*)actorInfo->Transforms.data(), sizeof(FTransform) * len);

			mActors.insert({std::string(actorInfo->Name).c_str(),std::move(actorInfo)});
			for (size_t k = 0; k < actorInfo->StaticMeshNames.size(); k++)
			{
				std::string t(actorInfo->StaticMeshNames[k]);
				meshName.insert(std::move(t.c_str()));
			}
		}
		fin.close();
	}
	//LoadMeshData
	for (auto name : meshName)
	{
		std::string meshPath = std::string("..\\AlphaEngine\\Data\\Mesh\\").append(name.c_str()).append(".dat");
		AEngine::GetSingleton().GetAssetManager()->LoadStaticMesh(meshPath);
	}

	return true;
}

bool AScene::LoadLight(const std::string& Path)
{
	std::ifstream inFile(Path, std::ios::binary);
	if (inFile.is_open()) {
		inFile.read((char*)&DirectionalLight.Location, sizeof(glm::vec4));
		inFile.read((char*)&DirectionalLight.Direction, sizeof(glm::vec3));
		inFile.read((char*)&DirectionalLight.Brightness, sizeof(float));
	}
	else
	{
		return false;
	}
	inFile.close();
	return true;
}

std::unordered_map<std::string, ActorInfo*>& AScene::GetAllActor()
{
	return mActors;
}
