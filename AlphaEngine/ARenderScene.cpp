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
			meshData.Vertices[i].Normal.x = meshInfo->Normals[i].x;
			meshData.Vertices[i].Normal.y = meshInfo->Normals[i].y;
			meshData.Vertices[i].Normal.z = meshInfo->Normals[i].z;
			meshData.Vertices[i].Normal.w = meshInfo->Normals[i].w;
			meshData.Vertices[i].TangentX.x = meshInfo->TangentXs[i].x;
			meshData.Vertices[i].TangentX.y = meshInfo->TangentXs[i].y;
			meshData.Vertices[i].TangentX.z = meshInfo->TangentXs[i].z;
			meshData.Vertices[i].TangentX.w = meshInfo->TangentXs[i].w;			
			meshData.Vertices[i].TangentY.x = meshInfo->TangentYs[i].x;
			meshData.Vertices[i].TangentY.y = meshInfo->TangentYs[i].y;
			meshData.Vertices[i].TangentY.z = meshInfo->TangentYs[i].z;
			meshData.Vertices[i].UV.x = meshInfo->UVs[i].x;
			meshData.Vertices[i].UV.y = meshInfo->UVs[i].y;
		}
		meshData.Indices = meshInfo->Indices;
		mMeshDatas[Actor.first] = std::move(meshData);
	}

	HDRGeo = std::make_unique<MeshData>();
	HDRTriangle = std::make_shared<ARenderItem>();
	HDRGeo->Indices = { 0,1,2 };
	HDRGeo->Vertices.resize(3);
	HDRGeo->Vertices[0].Pos = { -1.0f,1.0f,0.0f };
	HDRGeo->Vertices[1].Pos = { -1.0f,-3.0f,0.0f };
	HDRGeo->Vertices[2].Pos = { 3.0f,1.0f,0.0f };
	return mMeshDatas;
}
