#pragma once
#include "CommonResourceDefine.h"

class AssetManager
{
public:
	AssetManager();
	virtual ~AssetManager();

	void LoadStaticMesh(std::string& FilePath);
	StaticMeshInfo* FindAssetByActor(ActorInfo& Actor);
	//StaticMeshInfo GetAssetByName(const std::string& name);
private:
	std::unordered_map<std::string, StaticMeshInfo*> mMeshInfo;
};

