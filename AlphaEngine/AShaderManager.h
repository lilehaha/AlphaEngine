#pragma once
#include "stdafx.h"
#include "AShader.h"

class AShaderManager : public Singleton<AShaderManager>
{
public:
	AShaderManager();
	~AShaderManager();
	AShader* CompileShader(const std::wstring& FilePath);
public:
	std::map<std::wstring, AShader> mShaderResult;
};

