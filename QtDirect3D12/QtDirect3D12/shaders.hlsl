/**
 *	@file		shaders.hlsl
 *	@brief		シェーダー
 *	@author		kkllPreciel
 *	@date		2017/05/18
 *	@version	1.0
 */

/**
 *  @brief  頂点シェーダーの入力形式
 */
struct VSInput
{
    float3		position : POSITION;        ///< 座標
    float3		normal   : NORMAL;          ///< 法線ベクトル
    float2		uv       : TEXCOORD;        ///< UV座標
    uint        id       : SV_InstanceID;   ///< インスタンスID
};

/**
 *  @brief  頂点シェーダーの出力形式
 */
struct VSOutput
{
    float4 position : SV_POSITION;  ///< 座標
    float3 normal   : COLOR0;       ///< ワールド空間での法線ベクトル
    float2 uv       : TEXCOORD;     ///< UV座標
    float4 color    : COLOR1;       ///< 色
    float4 view     : COLOR2;       ///< 視点へのベクトル
};

/**
 *  @brief  ピクセルシェーダーの出力形式
 */
struct PSOutput
{
    float4 color : SV_TARGET0; ///< 色
};

/**
 *  @brief  インスタンス毎のデータ
 */
struct InstanceBuffer
{
    float4x4 world; ///< ワールド行列
};

StructuredBuffer<InstanceBuffer> cbv : register(t0); ///< インスタンス毎のデータリスト

/**
 *  @brief  コンスタントバッファ
 */
cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;         ///< ワールド行列(列優先行列)
    float4x4 view;          ///< ビュー行列(列優先行列)
    float4x4 projection;    ///< プロジェクション行列(列優先行列)
};

/**
 *  @brief  テクスチャ
 */
Texture2D g_texture : register(t0);

/**
 *  @brief  サンプラー
 */
SamplerState g_sampler : register(s0);

/**
 *  @brief  エントリーポイント
 */
VSOutput VSMain(VSInput input)
{
    VSOutput result;

    float4 pos = float4(input.position, 1.0);

    pos = mul(cbv[input.id].world, pos);
    pos = mul(world, pos);
    pos = mul(view, pos);
    pos = mul(projection, pos);

    result.position = pos;
    result.color = float4(input.uv.x, input.uv.y, 1.0, 1.0);
    result.uv = input.uv;

    // 視点へのベクトル
    result.view = -mul(view, mul(world, mul(cbv[input.id].world, pos)));
    
    // ビュー空間での法線ベクトル(近似値)
    result.normal = mul(view, mul(world, mul(cbv[input.id].world, pos)));

    return result;
}


















































































































// defines
#define PI 3.14159265359
#define PI2 6.28318530718
#define RECIPROCAL_PI 0.31830988618
#define RECIPROCAL_PI2 0.15915494
#define LOG2 1.442695
#define EPSILON 1e-6

struct IncidentLight
{
    float3 color;
    float3 direction;
    bool visible;
};

struct ReflectedLight
{
    float3 directDiffuse;
    float3 directSpecular;
    float3 indirectDiffuse;
    float3 indirectSpecular;
};

struct GeometricContext
{
    float3 position;
    float3 normal;
    float3 viewDir;
};

struct Material
{
    float3 diffuseColor;
    float specularRoughness;
    float3 specularColor;
};

// lights

bool testLightInRange(const in float lightDistance, const in float cutoffDistance)
{
    return any(bool2(cutoffDistance == 0.0, lightDistance < cutoffDistance));
}

float punctualLightIntensityToIrradianceFactor(const in float lightDistance, const in float cutoffDistance, const in float decayExponent)
{
    if (decayExponent > 0.0)
    {
        return pow(saturate(-lightDistance / cutoffDistance + 1.0), decayExponent);
    }

    return 1.0;
}

struct DirectionalLight
{
    float3 direction;
    float3 color;
};

void getDirectionalDirectLightIrradiance(const in DirectionalLight directionalLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    directLight.color = directionalLight.color;
    directLight.direction = directionalLight.direction;
    directLight.visible = true;
}

struct PointLight
{
    float3 position;
    float3 color;
    float distance;
    float decay;
};

void getPointDirectLightIrradiance(const in PointLight pointLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = pointLight.position - geometry.position;
    directLight.direction = normalize(L);

    float lightDistance = length(L);
    if (testLightInRange(lightDistance, pointLight.distance))
    {
        directLight.color = pointLight.color;
        directLight.color *= punctualLightIntensityToIrradianceFactor(lightDistance, pointLight.distance, pointLight.decay);
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0, 0, 0);
        directLight.visible = false;
    }
}

struct SpotLight
{
    float3 position;
    float3 direction;
    float3 color;
    float distance;
    float decay;
    float coneCos;
    float penumbraCos;
};

void getSpotDirectLightIrradiance(const in SpotLight spotLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = spotLight.position - geometry.position;
    directLight.direction = normalize(L);

    float lightDistance = length(L);
    float angleCos = dot(directLight.direction, spotLight.direction);

    if (all(bool2(angleCos > spotLight.coneCos, testLightInRange(lightDistance, spotLight.distance))))
    {
        float spotEffect = smoothstep(spotLight.coneCos, spotLight.penumbraCos, angleCos);
        directLight.color = spotLight.color;
        directLight.color *= spotEffect * punctualLightIntensityToIrradianceFactor(lightDistance, spotLight.distance, spotLight.decay);
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0, 0, 0);
        directLight.visible = false;
    }
}

// light uniforms
#define LIGHT_MAX 4

// BRDFs

// Normalized Lambert
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor / PI;
}

float3 F_Schlick(float3 specularColor, float3 H, float3 V)
{
    return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

float D_GGX(float a, float dotNH)
{
    float a2 = a * a;
    float dotNH2 = dotNH * dotNH;
    float d = dotNH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL)
{
    float k = a * a * 0.5 + EPSILON;
    float gl = dotNL / (dotNL * (1.0 - k) + k);
    float gv = dotNV / (dotNV * (1.0 - k) + k);
    return gl * gv;
}

// Cook-Torrance
float3 SpecularBRDF(const in IncidentLight directLight, const in GeometricContext geometry, float3 specularColor, float roughnessFactor)
{

    float3 N = geometry.normal;
    float3 V = geometry.viewDir;
    float3 L = directLight.direction;

    float dotNL = saturate(dot(N, L));
    float dotNV = saturate(dot(N, V));
    float3 H = normalize(L + V);
    float dotNH = saturate(dot(N, H));
    float dotVH = saturate(dot(V, H));
    float dotLV = saturate(dot(L, V));
    float a = roughnessFactor * roughnessFactor;

    float D = D_GGX(a, dotNH);
    float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
    float3 F = F_Schlick(specularColor, V, H);
    return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

// RenderEquations(RE)
void RE_Direct(const in IncidentLight directLight, const in GeometricContext geometry, const in Material material, inout ReflectedLight reflectedLight)
{
    float dotNL = saturate(dot(geometry.normal, directLight.direction));
    float3 irradiance = dotNL * directLight.color;

  // punctual light
    irradiance *= PI;

    reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
    reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}

/**
 *  @brief  エントリーポイント
 */
PSOutput PSMain(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    output.color = input.color;
    output.color = g_texture.Sample(g_sampler, input.uv);

    GeometricContext geometry;
    geometry.position = -input.view;
    geometry.normal = normalize(input.normal);
    geometry.viewDir = normalize(input.view);

    float metallic = 0.5;
    float roughness = 0.5;
    float3 albedo = float3(1, 1, 1);

    Material material;
    material.diffuseColor = lerp(albedo, (float3) 0, metallic);
    material.specularColor = lerp((float3)0.04, albedo, metallic);
    material.specularRoughness = roughness;

  // Lighting

    ReflectedLight reflectedLight = (ReflectedLight) 0;
    float3 emissive = (float3) 0;
    float opacity = 1.0;

    IncidentLight directLight;

    DirectionalLight directionalLights[LIGHT_MAX];
    int numDirectionalLights = LIGHT_MAX;

  // directional light
    for (int i = 0; i < LIGHT_MAX; ++i)
    {
        directionalLights[i].direction = float3(0, 0, 1);
        directionalLights[i].color = float3(1, 1, 1);

        if (i >= numDirectionalLights)
            break;
        getDirectionalDirectLightIrradiance(directionalLights[i], geometry, directLight);
        RE_Direct(directLight, geometry, material, reflectedLight);
    }

    float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;

    output.color = float4(outgoingLight, opacity);
    return output;
}