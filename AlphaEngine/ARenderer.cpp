#include "stdafx.h"
#include "ARenderer.h"
#include "AEngine.h"

ARenderer::ARenderer() : mRHIBuilder(nullptr),mRHI(nullptr)
{
	mWorld = glm::identity<glm::mat4x4>();
}

ARenderer::~ARenderer()
{

}

bool ARenderer::Init()
{
	mRHIBuilder = std::make_unique<RHIBuilder>();
	mRHI = mRHIBuilder->CreateRHI();
	mRenderScene = std::make_shared<ARenderScene>();
	if (!mRHI->Init())
	{
		return false;
	}
	return true;
}

void ARenderer::Render()
{
	BuildRenderItemTrans(mRenderScene);
	int actorIndex = 0;
	for (auto&& actorPair : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		mRHI->Update(actorIndex, mRenderScene->mRenderItem[actorPair.first].get());
		actorIndex++;
	}

	mRHI->Draw(mRenderScene);
}

void ARenderer::RenderStart()
{
	mRHI->ResetCommand("Null");
	mRHI->RenderFrameBegin(mRenderScene);


	for (auto&& actorPair : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		mRHI->CreateCbHeapsAndSrv(actorPair.first, actorPair.second->StaticMeshNames[0], mRenderScene->mRenderItem[actorPair.first].get(), mRenderScene);
	}
	mRHI->ExecuteCommandLists();
}

void ARenderer::BuildRenderItemTrans(std::shared_ptr<ARenderScene> sceneResource)
{
	for (auto&& Actor : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		glm::qua<float> rotation = glm::qua<float>(
			Actor.second->Transforms[0].Rotation.w,
			Actor.second->Transforms[0].Rotation.x,
			Actor.second->Transforms[0].Rotation.y,
			Actor.second->Transforms[0].Rotation.z
			);
		glm::vec3 location = glm::vec3(
			Actor.second->Transforms[0].Location.x,
			Actor.second->Transforms[0].Location.y,
			Actor.second->Transforms[0].Location.z
		);
		glm::vec3 scale = glm::vec3(
			Actor.second->Transforms[0].Scale3D.x,
			Actor.second->Transforms[0].Scale3D.y,
			Actor.second->Transforms[0].Scale3D.z
		);

		glm::mat4x4 Rotation = glm::mat4_cast(rotation);
		glm::mat4x4 Translate = glm::identity<glm::mat4x4>();
		Translate = glm::translate(Translate, location);
		glm::mat4x4 Scale = glm::identity<glm::mat4x4>();
		Scale = glm::scale(Scale, scale);

		glm::mat4x4 W = Translate * Rotation * Scale;
		sceneResource->mRenderItem[Actor.first]->mRotation = glm::transpose(Rotation);
		sceneResource->mRenderItem[Actor.first]->mScale = glm::transpose(Scale);
		sceneResource->mRenderItem[Actor.first]->mWorld = glm::transpose(W * mWorld);
	}
}
