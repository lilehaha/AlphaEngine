#pragma once
#include "stdafx.h"
#include "CommonResourceDefine.h"
#include "ARenderItem.h"

class ARenderScene
{
public:
	std::unordered_map<std::string, MeshData>& BuildMeshData();
public:
	std::unordered_map<std::string, MeshData> mMeshDatas;
	std::map <std::string, std::shared_ptr<ARenderItem>> mRenderItem;
};

