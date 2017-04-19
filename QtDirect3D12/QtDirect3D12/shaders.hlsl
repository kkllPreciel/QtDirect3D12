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
    float4x4 world;         ///< ���[���h�s��
    float4x4 view;          ///< �r���[�s��
    float4x4 projection;    ///< �v���W�F�N�V�����s��
};

// �s�N�Z���V�F�[�_�ւ̓��� = �o�[�e�b�N�X�V�F�[�_�̏o��
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
	PSInput result;

    result.position = mul(position, world);
	result.color = color;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}
