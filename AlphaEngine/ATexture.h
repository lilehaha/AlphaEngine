#pragma once
#include "stdafx.h"

enum class E_TexType
{
	Diffuse,
	Normal,
	Default
};

class ATexture
{
public:
	ATexture(E_TexType Type);
	~ATexture() = default;

	std::string mName;
	std::wstring mFilePath;

	ComPtr<ID3D12Resource> mResource = nullptr;
	ComPtr<ID3D12Resource> mUploadHeap = nullptr;

	E_TexType mType;
};

