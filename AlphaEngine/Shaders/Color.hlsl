//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************
#include "CommonHeader.hlsli"

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

SamplerState gSamplerWrap : register(s0);


struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
	float4 UV : UV;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
	float4 UV : UV;
};

[RootSignature(Common_RootSig)]
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.Normal = mul(vin.Normal, gRotation);

	float3 PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

	vout.PosH = mul(float4(PosW, 1.0f), gViewProj);

	vout.UV = vin.UV;

	return vout;
}

[RootSignature(Common_RootSig)]
float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gSamplerWrap, pin.UV);
	float4 normalMap = gNormalMap.Sample(gSamplerWrap, pin.UV);
	//float4 finalCol = diffuseAlbedo + normalMap;
	float4 finalCol = diffuseAlbedo;
	return pow(finalCol * 0.5f + 0.5f, 1 / 2.2f);
	//return pow(pin.Normal * 0.5f + 0.5f, 1 / 2.2f);
}


