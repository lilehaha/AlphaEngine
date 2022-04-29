#include "stdafx.h"
#include "ARenderScene.h"
#include "AEngine.h"

std::unordered_map<std::string, MeshData>& ARenderScene::BuildMeshData()
{
	for (auto&& Actor : AEngine::GetSingleton().GetScene()->GetAllActor())
	{
		StaticMeshInfo* meshInfo = AEngine::GetSingleton().GetAssetManager()->FindAssetByActor(*Actor.second);
		MeshData meshData;

		int VerticesVecLen = meshInfo->Vertices.size();
		int IndicesVecLen = meshInfo->Indices.size();

		meshData.Vertices.resize(VerticesVecLen);
		meshData.Indices.resize(IndicesVecLen);

		for (int i = 0; i < VerticesVecLen; i++) {
			meshData.Vertices[i].Pos.x = meshInfo->Vertices[i].x;
			meshData.Vertices[i].Pos.y = meshInfo->Vertices[i].y;
			meshData.Vertices[i].Pos.z = meshInfo->Vertices[i].z;
			meshData.Vertices[i].Normal.w = meshInfo->Normals[i].w;
			meshData.Vertices[i].Normal.x = meshInfo->Normals[i].x;
			meshData.Vertices[i].Normal.y = meshInfo->Normals[i].y;
			meshData.Vertices[i].Normal.z = meshInfo->Normals[i].z;
		}
		meshData.Indices = meshInfo->Indices;
		mMeshDatas[Actor.first] = std::move(meshData);
	}

	return mMeshDatas;
}
