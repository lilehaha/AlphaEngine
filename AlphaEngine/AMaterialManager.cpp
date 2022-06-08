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
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\ShadowMap.hlsl";
		mMaterial.Name = "Shadow";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::Shadow, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
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
			{ "TANGENTX", 0, FORMAT_R32G32B32A32_FLOAT, 0, 44, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENTY", 0, FORMAT_R32G32B32_FLOAT, 0, 60, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "UV", 0, FORMAT_R32G32_FLOAT, 0, 72, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
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
	else if(MaterialName == "Bloom")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, FORMAT_R32G32B32A32_FLOAT, 0, 12, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, FORMAT_R32G32B32A32_FLOAT, 0, 28, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENTX", 0, FORMAT_R32G32B32A32_FLOAT, 0, 44, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENTY", 0, FORMAT_R32G32B32_FLOAT, 0, 60, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "UV", 0, FORMAT_R32G32_FLOAT, 0, 72, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		mMaterial.Name = "Bloom";
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\HightLight.hlsl";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::Bloom, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["Bloom"] = mMaterial;

		return mMaterialMap["Bloom"];
	}
	else if (MaterialName == "BloomSetup")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mMaterial.Name = "BloomSetup";
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\BloomSetup.hlsl";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::BloomSetup, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["BloomSetup"] = mMaterial;

		return mMaterialMap["BloomSetup"];
	}else if (MaterialName == "BloomUp")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mMaterial.Name = "BloomUp";
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\BloomUp.hlsl";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::BloomSetup, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["BloomUp"] = mMaterial;

		return mMaterialMap["BloomUp"];
	}else if (MaterialName == "BloomDown")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mMaterial.Name = "BloomDown";
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\BloomDown.hlsl";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::BloomDown, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["BloomDown"] = mMaterial;

		return mMaterialMap["BloomDown"];
	}else if (MaterialName == "BloomSunMerge")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mMaterial.Name = "BloomSunMerge";
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\BloomSunMerge.hlsl";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::BloomSunMerge, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["BloomSunMerge"] = mMaterial;

		return mMaterialMap["BloomSunMerge"];
	}else if (MaterialName == "ToneMapping")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mMaterial.Name = "ToneMapping";
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\ToneMapping.hlsl";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::ToneMapping, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["ToneMapping"] = mMaterial;

		return mMaterialMap["ToneMapping"];
	}
	else if (MaterialName == "Glitch")
	{
		std::vector<INPUT_ELEMENT_DESC> mInputLayout;
		mInputLayout =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mMaterial.Name = "Glitch";
		mMaterial.mShaderFilePath = L"..\\AlphaEngine\\Shaders\\Glitch.hlsl";
		mMaterial.mPSO = mPSOManager->CreatePSO(E_PSOType::Glitch, std::move(mInputLayout), AShaderManager::GetSingleton().CompileShader(mMaterial.mShaderFilePath));
		mMaterialMap["Glitch"] = mMaterial;

		return mMaterialMap["Glitch"];
	}
}
