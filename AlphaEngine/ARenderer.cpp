#include "stdafx.h"
#include "ARenderer.h"

ARenderer::ARenderer() : mRHIBuilder(nullptr),mRHI(nullptr)
{
}

ARenderer::~ARenderer()
{

}

bool ARenderer::Init()
{
	mRHIBuilder = std::make_unique<RHIBuilder>();
	mRHI = mRHIBuilder->CreateRHI();
	if (!mRHI->Init())
	{
		return false;
	}
	return true;
}

void ARenderer::Render()
{
	mRHI->Draw();
	mRHI->Update();
}
