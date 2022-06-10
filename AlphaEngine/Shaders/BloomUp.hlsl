#include "CommonHeader.hlsli"

struct VertexIn
{
	float3 PosL  : POSITION;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
};

float2 Circle(float Start, float Points, float Point)
{
	float Radians = (2.0f * 3.141592f * (1.0f / Points)) * (Start + Point);
	return float2(cos(Radians), sin(Radians));
}

[RootSignature(Common_RootSig)]
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

[RootSignature(Common_RootSig)]
float4 PS(VertexOut pin) : SV_Target
{
	int X = floor(pin.PosH.x);
	int Y = floor(pin.PosH.y);
	float4 OutColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float BloomWeightScalar = 1.0f / 5.0f;
	float BloomWeightScalar1 = 1.0f / 5.0f;
	float BloomUpScale = 1.32f;

	float2 Tex;
	Tex.x = 1.0f * X / RenderTargetSize[2];
	Tex.y = 1.0f * Y / RenderTargetSize[3];
	float DeltaU = 1.0f / RenderTargetSize[2];
	float DeltaV = 1.0f / RenderTargetSize[3];
	float2 DeltaUV = float2(DeltaU, DeltaV);

	float StartDinominator = 7.0f;
	float Start = 2.0 / StartDinominator;
	float4 ColorsUp[7];
	float4 ColorsDown[7];
	float4 ColorUpTotal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 ColorDownTotal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < StartDinominator; i++)
	{
		ColorsUp[i] = gBloomUp.Sample(gSamBloom, Tex + DeltaUV * BloomUpScale * Circle(Start, StartDinominator, (float)i));
		ColorsDown[i] = gBloomDown.Sample(gSamBloom, Tex + DeltaUV * BloomUpScale * Circle(Start, StartDinominator, (float)i));
		ColorUpTotal += ColorsUp[i];
		ColorDownTotal += ColorsDown[i];
	}
	float4 BloomWight = float4(BloomWeightScalar, BloomWeightScalar, BloomWeightScalar, 0.0f);
	float4 BloomWight1 = float4(BloomWeightScalar1, BloomWeightScalar1, BloomWeightScalar1, 0.0f);

	OutColor = ColorUpTotal * BloomWight + ColorDownTotal * BloomWight1;
	OutColor.a = 0.0f;
	return OutColor;
}
