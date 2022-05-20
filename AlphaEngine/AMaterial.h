#pragma once
#include "stdafx.h"
#include "APSO.h"
#include "CommonResourceDefine.h"

class AMaterial
{
public:
	std::string Name;
	int MatCBIndex = -1;
	int DiffuseSrvHeapIndex = -1;
	int NormalSrvHeapIndex = -1;

	std::wstring mShaderFilePath;
	APSO mPSO;

	MatConstants mMatConstants;
};

