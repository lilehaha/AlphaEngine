#pragma once
#include "stdafx.h"
#include "RHIBuilder.h"
#include "ARenderScene.h"

class ARenderer
{
public:
	ARenderer();
	virtual ~ARenderer();

	bool Init();
	void Render();
	void RenderStart();
	void UpdateShadowTransform(std::shared_ptr<ARenderScene> RenderScene);
	void UpdateRenderItemTrans(std::shared_ptr<ARenderScene> sceneResource);

	void BasePass(int RTVNumber, const std::string& PSOName);
	void ShadowPass();
	void HDRPass();
	void BloomPass(int& postProcessCount);
	void ToneMapPass(int RTVNumber, const std::string& PSOName);
	void PostProcessPass(int& index, const std::string& PSOName);
private:
	std::unique_ptr<RHIBuilder> mRHIBuilder;
	std::shared_ptr<ARHI> mRHI;
	std::shared_ptr<ARHIResource> mRHIResource = nullptr;
	std::shared_ptr<ARenderResource> mShadowResource = nullptr;
	std::shared_ptr<ARenderResource> mHDRResource = nullptr;
	std::shared_ptr<ARenderScene> mRenderScene;
	glm::mat4x4 mWorld;
	bool bisBloomDown = true;
};

