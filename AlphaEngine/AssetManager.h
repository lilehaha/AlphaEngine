#pragma once
#include "CommonResourceDefine.h"
#include "ATexture.h"

class AssetManager
{
public:
	AssetManager();
	virtual ~AssetManager();

	void LoadStaticMesh(std::string& FilePath);
	StaticMeshInfo* FindAssetByActor(ActorInfo& Actor);

	void LoadTextures();
	std::vector<std::shared_ptr<ATexture>>& GetDiffuseTextures();
	std::vector<std::shared_ptr<ATexture>>& GetNormalTextures();
private:
	std::unordered_map<std::string, StaticMeshInfo*> mMeshInfo;
	std::vector<std::shared_ptr<ATexture>> mDiffuseTextures;
	std::vector<std::shared_ptr<ATexture>> mNormalTextures;
};

