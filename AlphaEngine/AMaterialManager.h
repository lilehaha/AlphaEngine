#pragma once
#include "stdafx.h"
#include "AMaterial.h"

class AMaterialManager : public Singleton<AMaterialManager>
{
public:
	const AMaterial& GetMaterial(const std::string& MaterialName);
private:
	AMaterial mMaterial;
	std::unique_ptr<APSOManager> mPSOManager = std::make_unique<APSOManager>();
	std::map<std::string, AMaterial> mMaterialMap;
};

