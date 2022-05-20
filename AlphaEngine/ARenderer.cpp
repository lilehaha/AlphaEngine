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
	UpdateShadowTransform(mRenderScene);
	UpdateRenderItemTrans(mRenderScene);
	int actorIndex = 0;
	for (auto&& actorPair : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		mRHI->Update(actorIndex,mRenderScene, mRenderScene->mRenderItem[actorPair.first].get());
		actorIndex++;
	}
	mRHI->Draw(mRenderScene);
}

void ARenderer::RenderStart()
{
	for (auto&& dTexture : AEngine::GetSingleton().GetAssetManager()->GetDiffuseTextures()) {
		mRHI->CreateTextureResource(mRenderScene, dTexture);
	}
	for (auto&& nTexture : AEngine::GetSingleton().GetAssetManager()->GetNormalTextures()) {
		mRHI->CreateTextureResource(mRenderScene, nTexture);
	}

	mRHI->ResetCommand("Null");
	mRHI->RenderFrameBegin(mRenderScene);

	for (auto&& actorPair : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		mRHI->CreateCbHeapsAndSrv(actorPair.first, actorPair.second->StaticMeshNames[0], mRenderScene->mRenderItem[actorPair.first].get(), mRenderScene);
	}
	mRHI->ExecuteCommandLists();
}

void ARenderer::UpdateShadowTransform(std::shared_ptr<ARenderScene> RenderScene)
{
	float Radius = 2500;
	glm::vec3 direction =  AEngine::GetSingleton().GetScene()->DirectionalLight.Direction;
	glm::vec3 lightPos = -2.0f * Radius * AEngine::GetSingleton().GetScene()->DirectionalLight.Direction;
	float Time = AEngine::GetSingleton().GetTotalTime() / 3;
	direction.x = direction.x * glm::cos(Time) - direction.y * glm::sin(Time);
	direction.y = direction.y * glm::cos(Time) + direction.x * glm::sin(Time);
	lightPos.x = lightPos.x * glm::cos(Time) - lightPos.y * glm::sin(Time);
	lightPos.y = lightPos.y * glm::cos(Time) + lightPos.x * glm::sin(Time);
	RenderScene->LightDirection = direction;
	glm::mat4x4 lightView = glm::lookAtLH(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 sphereCenterLS = MathHelper::Vector3TransformCoord(glm::vec3(0.0f, 0.0f, 0.0f), lightView);
	float l = sphereCenterLS.x - Radius;
	float b = sphereCenterLS.y - Radius;
	float n = sphereCenterLS.z - Radius;
	float r = sphereCenterLS.x + Radius;
	float t = sphereCenterLS.y + Radius;
	float f = sphereCenterLS.z + Radius;
	glm::mat4x4 lightProj = glm::orthoLH_ZO(l, r, b, t, n, f);
	glm::mat4 T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	RenderScene->LightVP = glm::transpose(lightProj * lightView);
	RenderScene->TLightVP = glm::transpose(T * lightProj * lightView);
}

void ARenderer::UpdateRenderItemTrans(std::shared_ptr<ARenderScene> sceneResource)
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
