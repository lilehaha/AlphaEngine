#pragma once
#include "stdafx.h"
#include "AEngine.h"
#include "AGameLogic.h"

class AGameInstance
{
public:
	void Init(HINSTANCE hInstance);
	void UpDate();
	void Destroy();
private:
	std::unique_ptr<AEngine> mEngine;
	std::unique_ptr<AGameLogic> mGameLogic;
};

