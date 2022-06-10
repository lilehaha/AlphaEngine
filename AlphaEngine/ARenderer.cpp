#include "stdafx.h"
#include "ARenderer.h"
#include "AEngine.h"
#include "AShadowResource.h"
#include "DXRHIResource.h"
#include "AHDRResource.h"

ARenderer::ARenderer() : mRHIBuilder(nullptr),mRHI(nullptr)
{
	mWorld = glm::identity<glm::mat4x4>();
}

ARenderer::~ARenderer()
{

}

bool ARenderer::Init()
{
	mRenderScene = std::make_shared<ARenderScene>();
	mRHIBuilder = std::make_unique<RHIBuilder>();
	mRHI = mRHIBuilder->CreateRHI();
	if (!mRHI->Init())
	{
		return false;
	}
	mShadowResource = mRHIBuilder->CreateShadowResource();
	mRHIResource = mRHIBuilder->CreateRHIResource();
	mHDRResource = mRHIBuilder->CreateHDRResource();
	return true;
}

void ARenderer::Render()
{
	mRHI->ResetCommand("RenderScene");
	UpdateShadowTransform(mRenderScene);
	UpdateRenderItemTrans(mRenderScene);
	int actorIndex = 0;
	for (auto&& actorPair : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		mRHI->Update(actorIndex,mRenderScene, mRenderScene->mRenderItem[actorPair.first].get());
		actorIndex++;
	}
	mRHI->Update(actorIndex, mRenderScene, mRenderScene->HDRTriangle.get());
	ShadowPass();
	BasePass(0, "Base");

	int postProcessCount = 1;
	HDRPass();
	if (bisBloomDown)
	{
		BloomPass(postProcessCount);
	}
	
	//PostProcessPass(postProcessCount, "ToneMapping");
	//ToneMapPass(postProcessCount, "Glitch");

	ToneMapPass(postProcessCount, "ToneMapping");
	

	mRHI->ExecuteCommandLists();
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
	mRHI->RenderFrameBegin(mRenderScene, "Shadow");
	for (auto&& RenderItem : mRenderScene->mRenderItem)
	{
		mRHI->ChangePSOState(AMaterialManager::GetSingleton().GetMaterial("Shadow"), AMaterialManager::GetSingleton().GetMaterial("Shadow").mPSO, AMaterialManager::GetSingleton().GetMaterial("Shadow").mShaderFilePath);
		mRHI->SetPipelineState(RenderItem.second, AMaterialManager::GetSingleton().GetMaterial("Shadow"));
	}
	std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->bIsBloomDown = bisBloomDown;
	for (auto&& actorPair : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		mRHI->CreateCbHeapsAndSrv(actorPair.first, actorPair.second->StaticMeshNames[0], mRenderScene->mRenderItem[actorPair.first].get(), mShadowResource.get(), mHDRResource.get(), mRenderScene);
	}
	mRHI->CreateCbHeapsAndSrv("HDRTriangle", "HDRTriangle", mRenderScene->HDRTriangle.get(), mShadowResource.get(), mHDRResource.get(), mRenderScene);
	mRHI->ExecuteCommandLists();
}

void ARenderer::UpdateShadowTransform(std::shared_ptr<ARenderScene> RenderScene)
{
	float Radius = 450;
	glm::vec3 direction =  AEngine::GetSingleton().GetScene()->DirectionalLight.Direction;
	glm::vec3 lightPos = -1.0f * Radius * AEngine::GetSingleton().GetScene()->DirectionalLight.Direction;
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

void ARenderer::BasePass(int RTVNumber, const std::string& PSOName)
{
	mRHI->BeginEvent(PSOName);
	mRHI->RSSetViewports(0.0f, 0.0f, (float)AEngine::GetSingleton().GetWindow()->GetWidth(), (float)AEngine::GetSingleton().GetWindow()->GetHeight(), 0.0f, 1.0f);
	mRHI->RSSetScissorRects(0, 0, (long)AEngine::GetSingleton().GetWindow()->GetWidth(), (long)AEngine::GetSingleton().GetWindow()->GetHeight());

	mRHI->ResourceBarrier(1, mRHIResource->BackBuffer(), RESOURCE_STATES::PRESENT, RESOURCE_STATES::RENDER_TARGET);

	mRHI->ClearAndSetRenderTatget(mRHIResource->CurrentBackBufferViewHand(), mRHIResource->CurrentDepthStencilViewHand(),
		1, mRHIResource->CurrentBackBufferViewHand(), true, mRHIResource->CurrentDepthStencilViewHand());

	mRHI->ChangePSOState(AMaterialManager::GetSingleton().GetMaterial(PSOName), AMaterialManager::GetSingleton().GetMaterial(PSOName).mPSO, AMaterialManager::GetSingleton().GetMaterial(PSOName).mShaderFilePath);
	for (auto&& renderItem : mRenderScene->mRenderItem)
	{
		mRHI->SetPipelineState(renderItem.second, AMaterialManager::GetSingleton().GetMaterial(PSOName));
		mRHI->Draw(renderItem.second, renderItem.first, false, true, RTVNumber, 1920, 1080);
	}
	mRHI->ResourceBarrier(1, mRHIResource->BackBuffer(), RESOURCE_STATES::RENDER_TARGET, RESOURCE_STATES::PRESENT);
	mRHI->EndEvent();
}

void ARenderer::ShadowPass()
{
	mRHI->BeginEvent("ShadowPass");
	mRHI->RSSetViewports(0.0f, 0.0f, 2048, 2048, 0.0f, 1.0f);
	mRHI->RSSetScissorRects(0, 0, 2048, 2048);
	mRHI->ResourceBarrier(1, std::dynamic_pointer_cast<AShadowResource>(mShadowResource)->GetResource(), RESOURCE_STATES::RESOURCE_STATE_GENERIC_READ, RESOURCE_STATES::DEPTH_WRITE);
	mRHI->ClearAndSetRenderTatget(0, std::dynamic_pointer_cast<AShadowResource>(mShadowResource)->DSV(),
		0, 0, false, std::dynamic_pointer_cast<AShadowResource>(mShadowResource)->DSV());
	for (auto&& RenderItem : mRenderScene->mRenderItem)
	{
		mRHI->ChangePSOState(AMaterialManager::GetSingleton().GetMaterial(RenderItem.second->MatName), AMaterialManager::GetSingleton().GetMaterial("Shadow").mPSO, AMaterialManager::GetSingleton().GetMaterial("Shadow").mShaderFilePath);
		mRHI->SetPipelineState(RenderItem.second, AMaterialManager::GetSingleton().GetMaterial("Shadow"));
		mRHI->Draw(RenderItem.second, RenderItem.first, true, false, 0, 1920, 1080);
	}
	mRHI->ResourceBarrier(1, std::dynamic_pointer_cast<AShadowResource>(mShadowResource)->GetResource(), RESOURCE_STATES::DEPTH_WRITE, RESOURCE_STATES::RESOURCE_STATE_GENERIC_READ);
	mRHI->EndEvent();
}

void ARenderer::HDRPass()
{
	mRHI->BeginEvent("HDRPass");
	mRHI->RSSetViewports(0.0f, 0.0f, (float)AEngine::GetSingleton().GetWindow()->GetWidth(), (float)AEngine::GetSingleton().GetWindow()->GetHeight(), 0.0f, 1.0f);
	mRHI->RSSetScissorRects(0, 0, (long)AEngine::GetSingleton().GetWindow()->GetWidth(), (long)AEngine::GetSingleton().GetWindow()->GetHeight());
	mRHI->ResourceBarrier(1, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->GetRTVResource(0), RESOURCE_STATES::COMMON, RESOURCE_STATES::RENDER_TARGET);
	mRHI->ClearAndSetRenderTatget(std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->RTV(0), std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->DSV(0),
		1, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->RTV(0), false, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->DSV(0));
	for (auto&& RenderItem : mRenderScene->mRenderItem)
	{
		mRHI->ChangePSOState(AMaterialManager::GetSingleton().GetMaterial(RenderItem.second->MatName), AMaterialManager::GetSingleton().GetMaterial("HDR").mPSO, AMaterialManager::GetSingleton().GetMaterial("HDR").mShaderFilePath);
		mRHI->SetPipelineState(RenderItem.second, AMaterialManager::GetSingleton().GetMaterial("HDR"));
		mRHI->Draw(RenderItem.second, RenderItem.first, false, false, 0, 1920, 1080);
	}
	mRHI->ResourceBarrier(1, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->GetRTVResource(0), RESOURCE_STATES::RENDER_TARGET, RESOURCE_STATES::COMMON);
	mRHI->EndEvent();
}

void ARenderer::BloomPass(int& postProcessCount)
{
	PostProcessPass(postProcessCount, "BloomSetup");
	PostProcessPass(postProcessCount, "BloomDown");
	PostProcessPass(postProcessCount, "BloomDown");
	PostProcessPass(postProcessCount, "BloomUp");
	PostProcessPass(postProcessCount, "BloomSunMerge");
}

void ARenderer::ToneMapPass(int RTVNumber, const std::string& PSOName)
{
	mRHI->BeginEvent(PSOName);
	mRHI->RSSetViewports(0.0f, 0.0f, (float)AEngine::GetSingleton().GetWindow()->GetWidth(), (float)AEngine::GetSingleton().GetWindow()->GetHeight(), 0.0f, 1.0f);
	mRHI->RSSetScissorRects(0, 0, (long)AEngine::GetSingleton().GetWindow()->GetWidth(), (long)AEngine::GetSingleton().GetWindow()->GetHeight());
	mRHI->ResourceBarrier(1, mRHIResource->BackBuffer(), RESOURCE_STATES::PRESENT, RESOURCE_STATES::RENDER_TARGET);
	mRHI->ClearAndSetRenderTatget(mRHIResource->CurrentBackBufferViewHand(), mRHIResource->CurrentDepthStencilViewHand(),
		1, mRHIResource->CurrentBackBufferViewHand(), true, mRHIResource->CurrentDepthStencilViewHand());

	mRHI->ChangePSOState(AMaterialManager::GetSingleton().GetMaterial(mRenderScene->HDRTriangle->MatName), AMaterialManager::GetSingleton().GetMaterial(PSOName).mPSO, AMaterialManager::GetSingleton().GetMaterial(PSOName).mShaderFilePath);
	mRHI->SetPipelineState(mRenderScene->HDRTriangle, AMaterialManager::GetSingleton().GetMaterial(PSOName));

	mRHI->Draw(mRenderScene->HDRTriangle, "HDRTriangle", false, true, RTVNumber, 1920, 1080);
	mRHI->ResourceBarrier(1, mRHIResource->BackBuffer(), RESOURCE_STATES::RENDER_TARGET, RESOURCE_STATES::PRESENT);
	mRHI->EndEvent();
}

void ARenderer::PostProcessPass(int& index, const std::string& PSOName)
{
	mRHI->BeginEvent(PSOName);
	mRHI->RSSetViewports(0.0f, 0.0f, (float)AEngine::GetSingleton().GetWindow()->GetWidth(), (float)AEngine::GetSingleton().GetWindow()->GetHeight(), 0.0f, 1.0f);
	mRHI->RSSetScissorRects(0, 0, (long)AEngine::GetSingleton().GetWindow()->GetWidth(), (long)AEngine::GetSingleton().GetWindow()->GetHeight());
	mRHI->ResourceBarrier(1, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->GetRTVResource(index), RESOURCE_STATES::COMMON, RESOURCE_STATES::RENDER_TARGET);
	mRHI->ClearAndSetRenderTatget(std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->RTV(index), std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->DSV(index),
		1, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->RTV(index), true, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->DSV(index));
	mRHI->ChangePSOState(AMaterialManager::GetSingleton().GetMaterial(mRenderScene->HDRTriangle->MatName), AMaterialManager::GetSingleton().GetMaterial(PSOName).mPSO, AMaterialManager::GetSingleton().GetMaterial(PSOName).mShaderFilePath);
	mRHI->SetPipelineState(mRenderScene->HDRTriangle, AMaterialManager::GetSingleton().GetMaterial(PSOName));

	mRHI->Draw(mRenderScene->HDRTriangle, "HDRTriangle", false, true, index, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->width[index], std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->height[index]);

	mRHI->ResourceBarrier(1, std::dynamic_pointer_cast<AHDRResource>(mHDRResource)->GetRTVResource(index), RESOURCE_STATES::RENDER_TARGET, RESOURCE_STATES::COMMON);
	index++;
	mRHI->EndEvent();
}
