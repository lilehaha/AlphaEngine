#pragma once
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
	void BuildRenderItemTrans(std::shared_ptr<ARenderScene> sceneResource);
private:
	std::unique_ptr<RHIBuilder> mRHIBuilder;
	std::shared_ptr<ARHI> mRHI;

	std::shared_ptr<ARenderScene> mRenderScene;
	glm::mat4x4 mWorld;
};

