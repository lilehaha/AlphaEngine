#pragma once
#include "ARHI.h"

class RHIBuilder
{
public:
	std::shared_ptr<ARHI>  CreateRHI();
	std::shared_ptr<ARHIResource> CreateRHIResource();
	std::shared_ptr<ARenderResource> CreateShadowResource();
	std::shared_ptr<ARenderResource> CreateHDRResource();
};

