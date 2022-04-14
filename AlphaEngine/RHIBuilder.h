#pragma once
#include "ARHI.h"

class RHIBuilder
{
public:
	RHIBuilder();
	virtual ~RHIBuilder();
	std::shared_ptr<ARHI>  CreateRHI();
};

