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
	FVector Location;
	FVector Scale3D;
};

struct StaticMeshInfo
{
	std::string Name;
	int NumVertices = 0;
	int NumTriangles = 0;
	int NumIndices = 0;
	std::vector<uint32_t> Indices;
	std::vector<FVector> Vertices;
	std::vector<FVector4> Normals;
	std::vector<FVector4> TangentXs;
	std::vector<FVector4> TangentYs;
	std::vector<FVector2D> UVs;
}; 

struct ActorInfo
{
	std::string Name;
	std::vector<std::string> StaticMeshNames;
	std::vector<FTransform> Transforms;
};

struct Vertex
{
	glm::vec3 Pos;
	glm::vec4 Color;
	glm::vec4 Normal;
};

struct MeshData
{
	std::vector<Vertex> Vertices;	
	std::vector<uint32_t> Indices;	
};

struct ObjectConstants
{
	glm::mat4x4 world = glm::identity<glm::mat4x4>();
	glm::mat4x4 Rotation = glm::identity<glm::mat4x4>();
};

struct PassConstants
{
	glm::mat4x4 viewProj = glm::identity<glm::mat4x4>();
	float Time;
};