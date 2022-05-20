#include "stdafx.h"
#include "AShader.h"

AShaderManager::AShaderManager()
{

}

AShaderManager::~AShaderManager()
{

}

AShader* AShaderManager::CompileShader(const std::wstring& FilePath)
{
	if (mShaderResult.find(FilePath) != mShaderResult.end())
	{
		return &mShaderResult[FilePath];
	}
	AShader shader;
	mShaderResult.insert({ FilePath,shader });

#ifdef PlatformWindows
	mShaderResult[FilePath].mvsByteCode = d3dUtil::CompileShader(FilePath, nullptr, "VS", "vs_5_1");
	mShaderResult[FilePath].mpsByteCode = d3dUtil::CompileShader(FilePath, nullptr, "PS", "ps_5_1");
#elif defined(PlatformIOS)

#endif
	return &mShaderResult[FilePath];
}
