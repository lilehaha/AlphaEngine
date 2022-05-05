//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
	float4x4 gRotation;
};

cbuffer cbPass : register(b1)
{
	float4x4 gViewProj;
	float gTime;
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

float4 PS(VertexOut pin) : SV_Target
{
	return pow(pin.Normal * 0.5f + 0.5f, 1 / 2.2f);
}


