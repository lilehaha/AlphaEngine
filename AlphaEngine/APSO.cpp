#include "stdafx.h"
#include "APSO.h"

APSO& APSOManager::CreatePSO(E_PSOType PSOType, std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader)
{
	switch (PSOType)
	{
	case E_PSOType::Base:
		mPSOs[PSOType] = BuildBasePSO(InputLayout, Shader);
		break;
	case E_PSOType::Shadow:
		mPSOs[PSOType] = BuildShadowPSO(InputLayout, Shader);
		break;	
	case E_PSOType::Bloom:
		mPSOs[PSOType] = BuildShadowPSO(InputLayout, Shader);
		break;
	default:
		mPSOs[PSOType] = APSO();
		break;
	}
	return mPSOs[PSOType];
}


APSO APSOManager::BuildBasePSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader)
{
#ifdef DXRender
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	mDXInputLayout.resize(InputLayout.size());
	for (int i = 0; i < InputLayout.size(); i++)
	{
		mDXInputLayout[i].AlignedByteOffset = InputLayout[i].AlignedByteOffset;
		mDXInputLayout[i].Format = DXGI_FORMAT(InputLayout[i].Format);
		mDXInputLayout[i].InputSlot = InputLayout[i].InputSlot;
		mDXInputLayout[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION(InputLayout[i].InputSlotClass);
		mDXInputLayout[i].InstanceDataStepRate = InputLayout[i].InstanceDataStepRate;
		mDXInputLayout[i].SemanticIndex = InputLayout[i].SemanticIndex;
		mDXInputLayout[i].SemanticName = InputLayout[i].SemanticName;
	}
	psoDesc.InputLayout = { mDXInputLayout.data(), (UINT)mDXInputLayout.size() };

	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(Shader->mvsByteCode->GetBufferPointer()),
		Shader->mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(Shader->mpsByteCode->GetBufferPointer()),
		Shader->mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	mPso.Name = "Base";
	mPso.PipelineState = psoDesc;
	return mPso;
#endif
	return APSO();
}

APSO APSOManager::BuildShadowPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader)
{
#ifdef DXRender
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	mDXInputLayout.resize(InputLayout.size());
	for (int i = 0; i < InputLayout.size(); i++)
	{
		mDXInputLayout[i].AlignedByteOffset = InputLayout[i].AlignedByteOffset;
		mDXInputLayout[i].Format = DXGI_FORMAT(InputLayout[i].Format);
		mDXInputLayout[i].InputSlot = InputLayout[i].InputSlot;
		mDXInputLayout[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION(InputLayout[i].InputSlotClass);
		mDXInputLayout[i].InstanceDataStepRate = InputLayout[i].InstanceDataStepRate;
		mDXInputLayout[i].SemanticIndex = InputLayout[i].SemanticIndex;
		mDXInputLayout[i].SemanticName = InputLayout[i].SemanticName;
	}
	psoDesc.InputLayout = { mDXInputLayout.data(), (UINT)mDXInputLayout.size() };

	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(Shader->mvsByteCode->GetBufferPointer()),
		Shader->mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(Shader->mpsByteCode->GetBufferPointer()),
		Shader->mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
	psoDesc.RasterizerState.DepthBias = 100000;
	psoDesc.RasterizerState.DepthBiasClamp = 0.0f;
	psoDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 0;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	mPso.Name = "Shadow";
	mPso.PipelineState = psoDesc;
	return mPso;
#endif
	return APSO();
}

APSO APSOManager::BuildBloomPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader)
{
#ifdef DXRender
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;

	for (int i = 0; i < InputLayout.size(); i++)
	{
		mDXInputLayout[i].AlignedByteOffset = InputLayout[i].AlignedByteOffset;
		mDXInputLayout[i].Format = DXGI_FORMAT(InputLayout[i].Format);
		mDXInputLayout[i].InputSlot = InputLayout[i].InputSlot;
		mDXInputLayout[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION(InputLayout[i].InputSlotClass);
		mDXInputLayout[i].InstanceDataStepRate = InputLayout[i].InstanceDataStepRate;
		mDXInputLayout[i].SemanticIndex = InputLayout[i].SemanticIndex;
		mDXInputLayout[i].SemanticName = InputLayout[i].SemanticName;
	}
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	psoDesc.InputLayout = { mDXInputLayout.data(),(UINT)mDXInputLayout.size() };

	psoDesc.VS = {
		reinterpret_cast<BYTE*>(Shader->mvsByteCode->GetBufferPointer()),
		Shader->mvsByteCode->GetBufferSize()
	};
	psoDesc.PS = {
		reinterpret_cast<BYTE*>(Shader->mpsByteCode->GetBufferPointer()),
		Shader->mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;

	mPso.Name = "Bloom";
	mPso.PipelineState = psoDesc;
	return mPso;
#endif
	return APSO();
}

