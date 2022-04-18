#pragma once
#include "RHIBuilder.h"

class ARenderer
{
public:
	ARenderer();
	virtual ~ARenderer();

	bool Init();
	void Render();
private:
	std::unique_ptr<RHIBuilder> mRHIBuilder;
	std::shared_ptr<ARHI> mRHI;
};

