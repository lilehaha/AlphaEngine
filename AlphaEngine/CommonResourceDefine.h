#pragma once
#include <vector>
#include <string>
#include "ADirectionalLight.h"

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
	std::vector<FVector> TangentYs;
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
	glm::vec4 TangentX;
	glm::vec3 TangentY;
	glm::vec2 UV;
};

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;
};

struct LightT
{
	glm::vec4 Location;
	glm::vec3 Direction;
	float Brightness;
};

struct ObjectConstants
{
	glm::mat4x4 world = glm::identity<glm::mat4x4>();
	glm::mat4x4 viewProj = glm::identity<glm::mat4x4>();
	glm::mat4x4 Rotation = glm::identity<glm::mat4x4>();
	glm::mat4x4 TexTransform = glm::identity<glm::mat4x4>();
	glm::mat4x4 LightVP = glm::identity<glm::mat4x4>();
	glm::mat4x4 TLightVP = glm::identity<glm::mat4x4>();
	LightT directionalLight;
	glm::vec3 CameraLoc;
	float Time;
};

struct MatConstants
{
	glm::vec4 Diffuse = { 1.0f,1.0f, 1.0f, 1.0f };
	glm::vec3 FresnelR0 = { 0.01f, 0.01f, 0.10f };
	float Roughness = 0.25f;
	glm::mat4x4 Transform = glm::identity<glm::mat4x4>();
};