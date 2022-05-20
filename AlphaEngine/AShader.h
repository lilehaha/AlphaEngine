#pragma once
#include "stdafx.h"

class AShader
{
public:
#ifdef PlatformWindows
	Microsoft::WRL::ComPtr<ID3DBlob> mvsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mpsByteCode = nullptr;
#elif defined(PlatformIOS)

#endif
};

class AShaderManager : public Singleton<AShaderManager>
{
public:
	AShaderManager();
	~AShaderManager();
	AShader* CompileShader(const std::wstring& FilePath);
public:
	std::map<std::wstring, AShader> mShaderResult;
};

