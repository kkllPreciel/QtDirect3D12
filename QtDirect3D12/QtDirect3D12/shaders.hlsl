//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

// �R���X�^���g�o�b�t�@
cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;        ///< ���[���h�s��(��D��s��)
    float4x4 view;         ///< �r���[�s��(��D��s��)
    float4x4 projection;   ///< �v���W�F�N�V�����s��(��D��s��)
};

// �s�N�Z���V�F�[�_�ւ̓��� = �o�[�e�b�N�X�V�F�[�_�̏o��
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(float3 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0)
{
	PSInput result;

    float4 pos = float4(position, 1.0);

    pos = mul(world, pos);
    pos = mul(view, pos);
    pos = mul(projection, pos);

    result.position = pos;
    result.color = float4(uv.x, uv.y, 1.0, 1.0);

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}
