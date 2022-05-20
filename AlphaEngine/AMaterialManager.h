#pragma once
#include "stdafx.h"
#include "AMaterial.h"

class AMaterialManager
{
public:
	const AMaterial& GetMaterial(const std::string& MaterialName);
private:
	AMaterial mMaterial;
	std::unique_ptr<APSOManager> mPSOManager;
	std::map<std::string, AMaterial> mMaterialMap;
};

