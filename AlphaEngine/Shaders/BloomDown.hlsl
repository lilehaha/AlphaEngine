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
	float DeltaU = 1.0f / RenderTargetSize[2];
	float DeltaV = 1.0f / RenderTargetSize[3];
	float2 DeltaUV = float2(DeltaU, DeltaV);
	float Width = RenderTargetSize[2];
	float Height = RenderTargetSize[3];
	int X = floor(pin.PosH.x);
	int Y = floor(pin.PosH.y);
	float BloomDownScale = 2.5f;
	float2 Tex;
	Tex.x = 1.0f * X / Width;
	Tex.y = 1.0f * Y / Height;
	float DoubleRadias = 14.0f;
	float StartRaduas = 2.0f / DoubleRadias;
	float4 Colors[14];
	float4 Color = gBloomDown.Sample(gSamBloom, Tex);
	for (int i = 0; i < DoubleRadias; i++)
	{
		Colors[i] = gBloomDown.Sample(gSamBloom, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, DoubleRadias, float(i)));
		Color = Color + Colors[i];
	}
	float Weight = 1.0f / 15.0f;
	Color = Weight * Color;
	return Color;
}