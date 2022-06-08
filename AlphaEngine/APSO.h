#pragma once
#include "stdafx.h"
#include "AShader.h"
#include "ARenderResource.h"

enum class E_PSOType
{
	Base = 0,
	Shadow = 1,
	Bloom = 2,
	BloomSetup = 3,
	BloomUp = 4,
	BloomDown = 5,
	BloomSunMerge = 6,
	ToneMapping = 7,
	Glitch = 8
};

enum INPUT_CLASSIFICATION
{
	INPUT_CLASSIFICATION_PER_VERTEX_DATA = 0,
	INPUT_CLASSIFICATION_PER_INSTANCE_DATA = 1
};

struct INPUT_ELEMENT_DESC
{
	LPCSTR SemanticName;
	UINT SemanticIndex;
	GI_FORMAT Format;
	UINT InputSlot;
	UINT AlignedByteOffset;
	INPUT_CLASSIFICATION InputSlotClass;
	UINT InstanceDataStepRate;
};

class APSO
{
public:
	std::string Name;
#ifdef DXRender
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineState;
#endif 
};

class APSOManager
{
public:
	APSO& CreatePSO(E_PSOType PSOType, std::vector<INPUT_ELEMENT_DESC> InputLayout,AShader* Shader);
	APSO BuildBasePSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildShadowPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildBloomPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildBloomSetupPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildBloomUpPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildBloomDownPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildBloomSunMergePSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildToneMappingPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
	APSO BuildGlitchPSO(std::vector<INPUT_ELEMENT_DESC> InputLayout, AShader* Shader);
private:
	std::unordered_map<E_PSOType, APSO> mPSOs;
	APSO mPso;
#ifdef DXRender
	std::vector<D3D12_INPUT_ELEMENT_DESC> mDXInputLayout;
#endif 
};

