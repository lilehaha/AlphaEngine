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
	float gTime;
};

cbuffer cbPass : register(b2)
{
	
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
};

[RootSignature(Common_RootSig)]
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.Normal = mul(vin.Normal, gRotation);

	float3 PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

	//PosW.x += sin(gTime) * vout.Normal.x * 10;
	//PosW.y += sin(gTime) * vout.Normal.y * 10;
	//PosW.z += sin(gTime) * vout.Normal.z * 10;

	vout.PosH = mul(float4(PosW, 1.0f), gViewProj);

	return vout;
}

[RootSignature(Common_RootSig)]
float4 PS(VertexOut pin) : SV_Target
{
	return pow(pin.Normal * 0.5f + 0.5f, 1 / 2.2f);
}


