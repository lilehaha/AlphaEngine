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
		float BloomDownScale = 1.35f;

	int X = floor(pin.PosH.x);
	int Y = floor(pin.PosH.y);

	float Width = RenderTargetSize[0] * 0.5f;
	float Height = RenderTargetSize[1] * 0.5f;

	float DeltaU = 1.0f / RenderTargetSize[0];
	float DeltaV = 1.0f / RenderTargetSize[1];

	float2 Tex;
	Tex.x = 1.0f * X / Width;
	Tex.y = 1.0f * Y / Height;


	float2 DeltaUV = float2(DeltaU, DeltaV);

	float DoubleRadias = 8.0f;
	float StartRaduas = 2.0f / DoubleRadias;
	float4 Colors[8];
	float4 Color = gBloomInput.Sample(gBloomInputSampler, Tex);
	for (int i = 0; i < DoubleRadias; i++)
	{
		Colors[i] = gBloomInput.Sample(gBloomInputSampler, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, DoubleRadias, float(i)));
		Color = Color + Colors[i];
	}

	float Weight = 1.0f / (DoubleRadias + 1) * 2.0f;
	Color = Weight * Color;

	return Color;
}