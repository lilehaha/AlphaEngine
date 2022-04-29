#pragma once
#include "stdafx.h"

class AResource
{
public:
	ID3D12Resource* Resource;
};

class ARHIResource {
public:
	virtual void Release() = 0;
	virtual unsigned __int64  CurrentBackBufferViewHand() { return 0; }
	virtual unsigned __int64  CurrentDepthStencilViewHand() { return 0; }
	virtual std::shared_ptr<AResource> BackBuffer() = 0;
};

