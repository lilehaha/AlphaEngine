#include "CommonHeader.hlsli"

struct VertexIn
{
	float3 PosL  : POSITION;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
};

[RootSignature(Common_RootSig)]
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float3 POSL = vin.PosL;
	float4x4 posW = mul(gWorld, gLightVP);
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(POSL, 1.0f), posW);
	return vout;
}

void PS(VertexOut pin)
{
}