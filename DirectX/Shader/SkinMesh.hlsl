Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

cbuffer global_0 : register(b0)
{
    float3 lightDir : packoffset(c0);
    float3 lightColor : packoffset(c1); //���C�g�̐F
    float3 cameraPos : packoffset(c2);
    matrix world : packoffset(c3);
    matrix wvp : packoffset(c7);
    matrix bones[256] : packoffset(c11);
};

cbuffer global_1 : register(b1)
{
    float3 ambient : packoffset(c0);
    float4 diffuse : packoffset(c1);
    float3 specular : packoffset(c2);
    float shininess : packoffset(c3);
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float3 WorldPos : POSITION;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float4 weight : BLENDWEIGHT, min16uint4 index : BLENDINDICES)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    matrix comb = (float4x4) 0;
    for (int i = 0; i < 4; ++i)
    {
        comb += bones[index[i]] * weight[i];
    }

    float4 position = mul(comb, pos);
    output.Pos = mul(wvp, position);
    float3 norm = mul(world, float4(normal, 0)).xyz;
    output.Normal = normalize(norm);
    output.UV = uv;
    output.WorldPos = mul(world, position).xyz;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 normal = input.Normal;
    float3 viewDir = normalize(cameraPos - input.WorldPos);

    float NL = dot(normal, -lightDir);

    //�g�U���ˌ�
    float3 diff = NL * diffuse.rgb;

    //���ˌ��x�N�g��
    float3 reflect = normalize(lightDir + 2 * NL * normal);
    //���ʔ��ˌ�
    float spec = pow(saturate(dot(reflect, viewDir)), shininess) * specular;

    float3 color = saturate(ambient + diff + spec) * lightColor;
    float4 texColor = tex.Sample(samplerState, input.UV);

    return float4(color * texColor.rgb, diffuse.a * texColor.a);
}