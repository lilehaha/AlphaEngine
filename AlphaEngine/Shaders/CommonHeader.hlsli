#define Common_RootSig\
"RootFlags( ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT )," \
"RootConstants(b0, num32BitConstants = 4),"\
"DescriptorTable(CBV(b1,numDescriptors = 2), visibility = SHADER_VISIBILITY_ALL),"\
"DescriptorTable(SRV(t0,numDescriptors = 3), visibility = SHADER_VISIBILITY_PIXEL),"\
"StaticSampler(s0," \
	"addressU = TEXTURE_ADDRESS_WRAP," \
	"addressV = TEXTURE_ADDRESS_WRAP," \
	"addressW = TEXTURE_ADDRESS_WRAP," \
	"filter = FILTER_MIN_MAG_MIP_POINT),"\
"StaticSampler(s1," \
	"addressU = TEXTURE_ADDRESS_BORDER," \
	"addressV = TEXTURE_ADDRESS_BORDER," \
	"addressW = TEXTURE_ADDRESS_BORDER," \
	"filter = FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,"\
	"mipLODBias =0 ,"\
	"maxAnisotropy = 16,"\
	"comparisonFunc = COMPARISON_LESS_EQUAL,"\
	"borderColor = STATIC_BORDER_COLOR_OPAQUE_BLACK)"\

struct Light {
	float4 Location;
	float3 Direction;
	float Strength;
};
float3 CameraLoc : register(b0);

cbuffer cbPerObject : register(b1)
{
	float4x4 gWorld;
	float4x4 gViewProj;
	float4x4 gRotation;
	float4x4 gTexTransform;
	float4x4 gLightVP;
	float4x4 gTLightVP;
	float gTime;
	Light light;
};

cbuffer cbMat : register(b2)
{
	float4 gDiffuseAlbedo;
	float3 gFresnelR0;
	float gRoughness;
	float4x4 gMatTransform;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gShadowMap : register(t2);

SamplerState gSamplerWrap : register(s0);
