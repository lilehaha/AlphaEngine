#include "stdafx.h"
#include "AMaterialManager.h"

const AMaterial& AMaterialManager::GetMaterial(const std::string& MaterialName)
{
	if (mMaterialMap.find(MaterialName) != mMaterialMap.end())
	{
		return mMaterialMap[MaterialName];
	}

	if (MaterialName == "Shadow")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, FORMAT_R32G32B32A32_FLOAT, 0, 12, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, FORMAT_R32G32B32A32_FLOAT, 0, 28, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "UV", 0, FORMAT_R32G32B32A32_FLOAT, 0, 44, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\Color.hlsl";
		mMaterial.Name = "Shadow";
		mMaterial.mMatConstants.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		mMaterial.mMatConstants.FresnelR0 = { 0.5f, 0.5f, 0.5f };
		mMaterial.mMatConstants.Roughness = 0.01f;
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::Base, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["Shadow"] = mMaterial;
		return mMaterialMap["Shadow"];
	}
	else if (MaterialName == "Base")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, FORMAT_R32G32B32A32_FLOAT, 0, 12, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, FORMAT_R32G32B32A32_FLOAT, 0, 28, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "UV", 0, FORMAT_R32G32B32A32_FLOAT, 0, 44, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\Color.hlsl";
		mMaterial.Name = "Base";
		mMaterial.mMatConstants.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		mMaterial.mMatConstants.FresnelR0 = { 0.5f, 0.5f, 0.5f };
		mMaterial.mMatConstants.Roughness = 0.01f;
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::Base, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["Base"] = mMaterial;
		return mMaterialMap["Base"];
	}
}
