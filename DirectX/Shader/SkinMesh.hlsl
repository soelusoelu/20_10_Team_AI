cbuffer global_0 : register(b0)
{
    matrix view : packoffset(c0);
    matrix proj : packoffset(c4);
    matrix world[256] : packoffset(c8);
};

cbuffer global_1 : register(b1)
{
    float3 ambient : packoffset(c0);
    float4 diffuse : packoffset(c1);
    float3 specular : packoffset(c2);
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float3 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float4 weight : BLENDWEIGHT, min16uint4 index : BLENDINDICES)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
        comb += world[index[i]] * weight[i];
    }

    output.Pos = mul(comb, float4(pos, 1));
    output.Pos = mul(view, output.Pos);
    output.Pos = mul(proj, output.Pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(diffuse.rgb, 1);
}