/**
 *  @file     app_shader.h
 *  @brief    アプリケーションで使用するシェーダーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/26
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <DirectXMath.h>
#include <Sein/Direct3D12/direct3d12_device.h>

namespace App
{
  /**
   *  @brief  アプリケーション用シェーダーインターフェイス
   */
  class IAppShader
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IAppShader() {};

    /**
     *  @brief  デストラクタ
     */
    virtual ~IAppShader() {};
    
    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IAppShader(const IAppShader& other) = delete;
    
    /**
     *  @brief  コピー代入演算子オペレータ
     *  @param  other:コピー代入元のインスタンス
     *  @return コピー代入後のインスタンス
     */
    IAppShader& operator = (const IAppShader& other) = delete;

    /**
     *  @brief  ビュー行列を設定する
     *  @param  view:ビュー行列
     */
    virtual void SetView(DirectX::XMMATRIX view) = 0;

    /**
     *  @brief  プロジェクション行列を設定する
     *  @param  projection:プロジェクション行列
     */
    virtual void SetProjection(DirectX::XMMATRIX projection) = 0;

    /**
     *  @brief  アプリケーション用シェーダーを作成する
     *  @param  device:デバイス
     *  @return アプリケーション用シェーダーインターフェイスへのポインタ
     */
    static std::unique_ptr<IAppShader> Create(Sein::Direct3D12::Device& device);
  };
};