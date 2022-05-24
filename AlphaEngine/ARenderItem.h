#pragma once
#include "stdafx.h"
#include "ABuffer.h"
#include "AMaterial.h"

class ARenderItem
{
public:
	glm::mat4x4 mWorld = glm::identity<glm::mat4x4>();
	glm::mat4x4 mRotation = glm::identity<glm::mat4x4>();
	glm::mat4x4 mScale = glm::identity<glm::mat4x4>();

	UINT mCBHeapIndex = -1;
	UINT mMatCBIndex = -1;
	UINT mSrvCBIndex = -1;
	std::unique_ptr<DXBuffer> mGeo = nullptr;
	std::string MatName;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	UINT BaseVertexLocation = 0;
};

