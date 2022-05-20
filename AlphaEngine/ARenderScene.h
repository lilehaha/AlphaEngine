#pragma once
#include "stdafx.h"
#include "CommonResourceDefine.h"
#include "ARenderItem.h"
#include "ATexture.h"

class ARenderScene
{
public:
	std::unordered_map<std::string, MeshData>& BuildMeshData();
public:
	std::unordered_map<std::string, MeshData> mMeshDatas;
	std::map <std::string, std::shared_ptr<ARenderItem>> mRenderItem;
	std::unordered_map<std::string, std::shared_ptr<ATexture>> mDiffuseTexs;
	std::unordered_map<std::string, std::shared_ptr<ATexture>> mNormalTexs;
	glm::mat4x4 LightVP;
	glm::mat4x4 TLightVP;
	glm::vec3 LightDirection;
};

