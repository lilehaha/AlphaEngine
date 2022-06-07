#include "CommonHeader.hlsli"

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
	float4 TangentX : TANGENTX;
	float3 TangentY : TANGENTY;
	float2 UV : UV;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 ShadowPosH : POSITION0;
	float3 PosW : POSITION1;
	float4 Color : COLOR;
	float3 NormalW : NORMALW;
	float3 NormalR : NORMALR;
	float3 TangentW : TANGENTX;
	float3 BiTangent : TANGENTY;
	float2 UV : UV;
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

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
	float cosIncidentAngle = saturate(dot(normal, lightVec));

	float f0 = 1.0f - cosIncidentAngle;
	float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

	return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	const float m = mat.Shininess * 256.0f;
	float3 halfVec = normalize(toEye + lightVec);

	float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);

	float3 specAlbedo = fresnelFactor * roughnessFactor;

	// Our spec formula goes outside [0,1] range, but we are 
	// doing LDR rendering.  So scale it down a bit.
	specAlbedo = specAlbedo / (specAlbedo + 1.0f);

	return (mat.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.Direction;

	// Scale light down by Lambert's cosine law.
	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.Strength * ndotl * 0.3f;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

[RootSignature(Common_RootSig)]
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(PosW, gViewProj);
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

	vout.NormalW = mul(vin.Normal.xyz, (float3x3)gWorld);
	vout.NormalR = mul(vin.Normal, gRotation).xyz;

	vout.Color = float4(vout.NormalR * 0.5 + 0.5, 1.0f);

	vout.TangentW = mul(vin.TangentX, gRotation).xyz;

	vout.BiTangent = mul(vin.TangentY, (float3x3)gRotation);

	vout.ShadowPosH = mul(PosW, gTLightVP);

	//float4 texUV = mul(float4(vin.UV, 0.0f, 1.0f), gTexTransform);
	//vout.UV = texUV.xy;
	vout.UV = vin.UV;

	return vout;
}

[RootSignature(Common_RootSig)]
float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gSamplerWrap, pin.UV);
	float4 normalMap = gNormalMap.Sample(gSamplerWrap, pin.UV);

	float3x3 TBN = float3x3(pin.TangentW, pin.BiTangent, pin.NormalW);
	float3 bumpedNormalW = normalize(mul((2.0f * normalMap - 1.0f).xyz, TBN));
	//bumpedNormalW = pin.NormalR;

	float shadowFactor = CalcShadowFactorPro(pin.ShadowPosH);

	float4 gAmbientLight = diffuseAlbedo * 0.03;
	float4 ambient = gAmbientLight * diffuseAlbedo;
	float3 toEyeW = normalize(CameraLoc.xyz - pin.PosW);

	float3 fresnelR0 = gFresnelR0;
	float  roughness = gRoughness;
	const float shininess = 1.0f - roughness;
	Material mat = { diffuseAlbedo, fresnelR0, roughness ,shininess };

	float4 directLight = float4(ComputeDirectionalLight(light, mat, bumpedNormalW, toEyeW),1.0f);
	//float4 finalCol = diffuseAlbedo + normalMap;
	//float4 finalCol = pin.Color;
	//float4 finalCol = diffuseAlbedo * (shadowFactor + 0.1);
	float4 finalCol = ambient + (shadowFactor + 0.1f) * directLight;

	//return finalCol;
	return pow(finalCol, 1 / 2.2f);
}

