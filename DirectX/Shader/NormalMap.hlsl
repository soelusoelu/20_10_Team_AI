Texture2D tex : register(t0);
Texture2D normalTex : register(t1);
SamplerState samplerState : register(s0);

cbuffer global_0 : register(b0)
{
    matrix world : packoffset(c0); //ワールド行列
    matrix wvp : packoffset(c4); //ワールドから射影までの変換行列
    float3 lightDir : packoffset(c8); //ライトの方向ベクトル
    float3 lightColor : packoffset(c9); //ライトの色
    float3 cameraPos : packoffset(c10); //カメラ位置
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
    float3 Tangent : TANGENT;
    float3 WorldPos : POSITION;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float3 tangent : TANGENT)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, pos);
    float3 norm = mul(world, float4(normal, 0)).xyz;
    output.Normal = normalize(norm);
    output.UV = uv;
    output.Tangent = tangent;
    output.WorldPos = mul(world, pos).xyz;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 normal = input.Normal;
    float3 viewDir = normalize(cameraPos - input.WorldPos);

    float NL = dot(normal, -lightDir);

    //拡散反射光
    float3 diff = NL * diffuse.rgb;

    //反射光ベクトル
    float3 reflect = normalize(lightDir + 2 * NL * normal);
    //鏡面反射光
    float spec = pow(saturate(dot(reflect, viewDir)), shininess) * specular;

    float3 color = saturate(ambient + diff + spec) * lightColor;
    float4 texColor = tex.Sample(samplerState, input.UV);

    return float4(color * texColor.rgb, diffuse.a * texColor.a);
}