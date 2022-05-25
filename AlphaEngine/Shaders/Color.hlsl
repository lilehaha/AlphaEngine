//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************
#include "CommonHeader.hlsli"

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
	float4 ShadowPosH : POSITION0;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
	float4 UV : UV;
};

float CalcShadowFactor(float4 shadowPosH)
{
    // Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;

    // Depth in NDC space.
    float depth = shadowPosH.z;

    uint width, height, numMips;
    gShadowMap.GetDimensions(0, width, height, numMips);
    float2 pixelPos = shadowPosH.xy * width;
    float depthInMap = gShadowMap.Load(int3(pixelPos, 0)).r;
    return depth > depthInMap ? 0 : 1;
}

float CalcShadowFactorPro(float4 shadowPosH)
{
    // Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;

    // Depth in NDC space.
    float depth = shadowPosH.z;

    uint width, height, numMips;
    gShadowMap.GetDimensions(0, width, height, numMips);

    // Texel size.
    float dx = 1.0f / (float)width;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx,  -dx), float2(0.0f,  -dx), float2(dx,  -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx,  +dx), float2(0.0f,  +dx), float2(dx,  +dx)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += gShadowMap.SampleCmpLevelZero(gSamShadow,
            shadowPosH.xy + offsets[i], depth).r;
    }

    return percentLit / 9.0f;
}

[RootSignature(Common_RootSig)]
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.Normal = mul(vin.Normal, gRotation);

	float3 PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

	vout.PosH = mul(float4(PosW, 1.0f), gViewProj);

	vout.UV = vin.UV;

	vout.Color = vin.Color;

	vout.ShadowPosH = mul(float4(PosW, 1.0f), gTLightVP);

	return vout;
}

[RootSignature(Common_RootSig)]
float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gSamplerWrap, pin.UV);
	float4 normalMap = gNormalMap.Sample(gSamplerWrap, pin.UV);

	float shadowFactor = CalcShadowFactorPro(pin.ShadowPosH);

	//float4 finalCol = diffuseAlbedo + normalMap;
	float4 finalCol = diffuseAlbedo * (shadowFactor + 0.1);

	return pow(finalCol * 0.5f + 0.5f, 1 / 2.2f);
}

