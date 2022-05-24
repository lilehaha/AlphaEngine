#include "stdafx.h"
#include "DXRHIResource.h"
#include "ARHIDX12.h"

DXRHIResource::DXRHIResource()
{
	mResource = std::make_shared<AResource>();
}

std::shared_ptr<AResource> DXRHIResource::BackBuffer()
{
	mResource->Resource = ARHIDX12::GetSingleton().CurrentBackBuffer();
	return mResource;
}

unsigned __int64 DXRHIResource::CurrentBackBufferViewHand()
{
	unsigned __int64 ptr = ARHIDX12::GetSingleton().CurrentBackBufferView().ptr;
	return ptr;
}

unsigned __int64 DXRHIResource::CurrentDepthStencilViewHand()
{
	unsigned __int64 ptr = ARHIDX12::GetSingleton().DepthStencilView().ptr;
	return ptr;
}
