#pragma once
#include "RHIBuilder.h"

class ARender
{
public:
	ARender();
	virtual ~ARender();

	bool Init();
	void Render();
private:
	std::unique_ptr<RHIBuilder> mRHIBuilder;
	std::shared_ptr<ARHI> mRHI;
};

