/**
 *	@file		pixel.hlsl
 *	@brief		ピクセルシェーダー
 *	@author		kkllPreciel
 *	@date		2017/08/04
 *	@version	1.0
 */

/**
 *  @brief  ピクセルシェーダーの入力形式
 */
struct PSInput
{
    float4 position : SV_POSITION;  ///< 座標
    float2 uv       : TEXCOORD;     ///< UV座標
    float4 color    : COLOR;        ///< 色
};

/**
 *  @brief  ピクセルシェーダーの出力形式
 */
struct PSOutput
{
    float4 color : SV_TARGET0; ///< 色
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
PSOutput main(PSInput input)
{
    PSOutput output = (PSOutput) 0;

    output.color = input.color;
    output.color = g_texture.Sample(g_sampler, input.uv);

    return output;
}