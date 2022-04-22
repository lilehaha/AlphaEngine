#pragma once
#include <vector>
#include <string>

struct FVector
{
public:
	float x;
	float y;
	float z;
};

struct FVector4
{
public:
	float x;
	float y;
	float z;
	float w;
};

struct FVector2D
{
public:
	float x;
	float y;
};

class FTransform
{
public:
	FVector4 Rotation;
	FVector Translation;
	FVector Scale3D;
};

struct StaticMeshLODInfo
{
	int NumVertices = 0;
	int NumTriangles = 0;
	int NumIndices = 0;
	std::vector<int32_t> Indices;
	std::vector<FVector> Vertices;
	std::vector<FVector4> Normals;
	std::vector<FVector4> TangentXs;
	std::vector<FVector4> TangentYs;
	std::vector<FVector2D> UVs;
};

struct StaticMeshInfo
{
	std::string Name;
	int NumLOD;
	std::vector<StaticMeshLODInfo> MeshLODInfo;
}; 

struct ActorInfo
{
	std::string Name;
	std::vector<std::string> StaticMeshNames;
	std::vector<FTransform> Transforms;
};

struct SceneInfo
{
	std::string Name;
	std::vector<ActorInfo> Actors;
};

