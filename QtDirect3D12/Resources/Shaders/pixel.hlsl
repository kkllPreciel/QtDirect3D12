/**
 *	@file		pixel.hlsl
 *	@brief		�s�N�Z���V�F�[�_�[
 *	@author		kkllPreciel
 *	@date		2017/08/04
 *	@version	1.0
 */

/**
 *  @brief  �s�N�Z���V�F�[�_�[�̓��͌`��
 */
struct PSInput
{
    float4 position : SV_POSITION;  ///< ���W
    float2 uv       : TEXCOORD;     ///< UV���W
    float4 color    : COLOR;        ///< �F
};

/**
 *  @brief  �s�N�Z���V�F�[�_�[�̏o�͌`��
 */
struct PSOutput
{
    float4 color : SV_TARGET0; ///< �F
};

/**
 *  @brief  �e�N�X�`��
 */
Texture2D g_texture : register(t0);

/**
 *  @brief  �T���v���[
 */
SamplerState g_sampler : register(s0);

/**
 *  @brief  �G���g���[�|�C���g
 */
PSOutput main(PSInput input)
{
    PSOutput output = (PSOutput) 0;

    output.color = input.color;
    output.color = g_texture.Sample(g_sampler, input.uv);

    return output;
}