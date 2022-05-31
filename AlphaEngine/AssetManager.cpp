#include "stdafx.h"
#include "AssetManager.h"

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{
	for (auto &&mesh: mMeshInfo)
	{
		ReleasePtr(mesh.second);
	}
}

void AssetManager::LoadStaticMesh(std::string& FilePath)
{
	std::ifstream fin(FilePath, std::ios::binary);
	StaticMeshInfo* meshInfo = new StaticMeshInfo;
	if (fin.is_open()) 
	{
		//name
		int strlen = 0;
		fin.read((char*)&strlen, sizeof(int32_t));
		fin.read((char*)meshInfo->Name.data(), sizeof(char) * strlen);
		//num
		fin.read((char*)&meshInfo->NumVertices, sizeof(int32_t));
		fin.read((char*)&meshInfo->NumTriangles, sizeof(int32_t));
		fin.read((char*)&meshInfo->NumIndices, sizeof(int32_t));

		int len;
		//indices
		fin.read((char*)&len, sizeof(int32_t));
		meshInfo->Indices.resize(len);
		fin.read((char*)meshInfo->Indices.data(), sizeof(int32_t)* len);
		//vertices
		fin.read((char*)&len, sizeof(int32_t));
		meshInfo->Vertices.resize(len);
		fin.read((char*)meshInfo->Vertices.data(), sizeof(FVector) * len);
		//normals
		fin.read((char*)&len, sizeof(int32_t));
		meshInfo->Normals.resize(len);
		fin.read((char*)meshInfo->Normals.data(), sizeof(FVector4) * len);
		//tangentXs
		fin.read((char*)&len, sizeof(int32_t));
		meshInfo->TangentXs.resize(len);
		fin.read((char*)meshInfo->TangentXs.data(), sizeof(FVector4) * len);
		//tangentYs
		fin.read((char*)&len, sizeof(int32_t));
		meshInfo->TangentYs.resize(len);
		fin.read((char*)meshInfo->TangentYs.data(), sizeof(FVector) * len);
		//UVs
		fin.read((char*)&len, sizeof(int32_t));
		meshInfo->UVs.resize(len);
		fin.read((char*)meshInfo->UVs.data(), sizeof(FVector2D) * len);
	}
	fin.close();
	mMeshInfo.insert({std::string(meshInfo->Name).c_str(), std::move(meshInfo)});
}

StaticMeshInfo* AssetManager::FindAssetByActor(ActorInfo& Actor)
{
	std::string str(Actor.StaticMeshNames[0].c_str());
	str.resize(str.size());
	auto iter = mMeshInfo.find(str);

	if (iter != mMeshInfo.end()) {
		return iter->second;
	}
	return nullptr;
}

std::vector<std::shared_ptr<ATexture>>& AssetManager::GetDiffuseTextures()
{
	return mDiffuseTextures;
}

std::vector<std::shared_ptr<ATexture>>& AssetManager::GetNormalTextures()
{
	return mNormalTextures;
}

void AssetManager::LoadTextures()
{
	auto tex_D_0 = std::make_unique<ATexture>(E_TexType::Diffuse);
	tex_D_0->mName = "Null";
	tex_D_0->mFilePath = L"..\\AlphaEngine\\Data\\Textures\\Metal_Gold_D.dds";
	mDiffuseTextures.push_back(std::move(tex_D_0));
	
	auto tex_N_0 = std::make_unique<ATexture>(E_TexType::Normal);
	tex_N_0->mName = "Null";
	tex_N_0->mFilePath = L"..\\AlphaEngine\\Data\\Textures\\Metal_Gold_N.dds";
	mNormalTextures.push_back(std::move(tex_N_0));

	auto tex_D_1 = std::make_unique<ATexture>(E_TexType::Diffuse);
	tex_D_1->mName = "Floor_400x400";
	tex_D_1->mFilePath = L"..\\AlphaEngine\\Data\\Textures\\Stone_D.dds";
	mDiffuseTextures.push_back(std::move(tex_D_1));

	auto tex_N_1 = std::make_unique<ATexture>(E_TexType::Normal);
	tex_N_1->mName = "Floor_400x400";
	tex_N_1->mFilePath = L"..\\AlphaEngine\\Data\\Textures\\Stone_N.dds";
	mNormalTextures.push_back(std::move(tex_N_1));
}

