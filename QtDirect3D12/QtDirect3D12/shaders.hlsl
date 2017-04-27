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

// コンスタントバッファ
cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;        ///< ワールド行列(列優先行列)
    float4x4 view;         ///< ビュー行列(列優先行列)
    float4x4 projection;   ///< プロジェクション行列(列優先行列)
};

// ピクセルシェーダへの入力 = バーテックスシェーダの出力
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
	PSInput result;

    position = mul(world, position);
    position = mul(view, position);
    position = mul(projection, position);

    result.position = position;
	result.color = color;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}
