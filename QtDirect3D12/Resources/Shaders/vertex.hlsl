/**
 *	@file		vertex.hlsl
 *	@brief		頂点シェーダー
 *	@author		kkllPreciel
 *	@date		2017/08/04
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
    float2 uv       : TEXCOORD;     ///< UV座標
    float4 color    : COLOR;        ///< 色
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
 *  @brief  エントリーポイント
 */
VSOutput main(VSInput input)
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

    return result;
}