#include "MeshCommonHeader.hlsli"

cbuffer OutLine : register(b1)
{
    float4 outLineColor;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return outLineColor;
}