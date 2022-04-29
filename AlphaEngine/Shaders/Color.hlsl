//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
};

cbuffer cbPass : register(b1)
{
	float4x4 gViewProj;
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

	float3 PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.PosH = mul(float4(PosW, 1.0f), gViewProj);
	//vout.PosH = mul(gWorld, gViewProj);

	vout.Color = vin.Normal*0.5f+0.5f;
	//vout.Color = vin.Color;
	//vout.Color = float4(PosW.x*0.5+0.5,1.0f,0.0f,1.0f);

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}


