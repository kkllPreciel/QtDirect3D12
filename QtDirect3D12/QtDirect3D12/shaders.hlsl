/**
 *	@file		shaders.hlsl
 *	@brief		�V�F�[�_�[
 *	@author		kkllPreciel
 *	@date		2017/05/18
 *	@version	1.0
 */

/**
 *  @brief  ���_�V�F�[�_�[�̓��͌`��
 */
struct VSInput
{
    float3		position : POSITION;        ///< ���W
    float3		normal   : NORMAL;          ///< �@���x�N�g��
    float2		uv       : TEXCOORD;        ///< UV���W
    uint        id       : SV_InstanceID;   ///< �C���X�^���XID
};

/**
 *  @brief  ���_�V�F�[�_�[�̏o�͌`��
 */
struct VSOutput
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
 *  @brief  �C���X�^���X���̃f�[�^
 */
struct InstanceBuffer
{
    float4x4 world; ///< ���[���h�s��
};

StructuredBuffer<InstanceBuffer> cbv : register(t0); ///< �C���X�^���X���̃f�[�^���X�g

/**
 *  @brief  �R���X�^���g�o�b�t�@
 */
cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;         ///< ���[���h�s��(��D��s��)
    float4x4 view;          ///< �r���[�s��(��D��s��)
    float4x4 projection;    ///< �v���W�F�N�V�����s��(��D��s��)
};

/**
 *  @brief  �e�N�X�`��
 */
Texture2D g_texture : register(t1);

/**
 *  @brief  �T���v���[
 */
SamplerState g_sampler : register(s0);

/**
 *  @brief  �G���g���[�|�C���g
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

    return result;
}

/**
 *  @brief  �G���g���[�|�C���g
 */
PSOutput PSMain(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    output.color = input.color;
    output.color = g_texture.Sample(g_sampler, input.uv);

    return output;
}