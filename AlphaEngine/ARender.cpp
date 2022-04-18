#include "stdafx.h"
#include "ARender.h"

ARender::ARender() : mRHIBuilder(nullptr),mRHI(nullptr)
{
}

ARender::~ARender()
{

}

bool ARender::Init()
{
	mRHIBuilder = std::make_unique<RHIBuilder>();
	mRHI = mRHIBuilder->CreateRHI();
	if (!mRHI->Init())
	{
		return false;
	}
	return true;
}

void ARender::Render()
{
	mRHI->Draw();
	mRHI->Update();
}
