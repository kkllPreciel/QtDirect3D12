/**
 *  @file     app_shader.cc
 *  @brief    アプリケーションで使用するシェーダーに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/26
 *  @version  1.0
 */

 // include
#include "app_shader.h"
#include <Sein/Direct3D12/constant_buffer.h>

namespace App
{
  namespace
  {
    /**
     *  @param  シェーダー用クラス
     */
    class AppShader final : public IAppShader
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      AppShader()
      {
    
      }
    
      /**
       *  @brief  デストラクタ
       */
      ~AppShader() override
      {

      }
    
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      AppShader(const AppShader& other) = delete;
    
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      AppShader& operator = (const AppShader& other) = delete;
    
      /**
       *  @brief  ムーブコンストラクタ
       *  @param  other:ムーブ元のインスタンス
       */
      AppShader(AppShader&& other) = delete;
    
      /**
       *  @brief  ムーブ代入演算子オペレータ
       *  @param  other:ムーブ代入元のインスタンス
       *  @return ムーブ代入後のインスタンス
       */
      AppShader& operator = (AppShader&& other) = delete;

      /**
       *  @brief  ビュー行列を設定する
       *  @param  view:ビュー行列
       */
      void SetView(DirectX::XMMATRIX view) override
      {
        DirectX::XMStoreFloat4x4(&(constant_buffer_.view), view);
      }
      
      /**
       *  @brief  プロジェクション行列を設定する
       *  @param  projection:プロジェクション行列
       */
      void SetProjection(DirectX::XMMATRIX projection) override
      {
        DirectX::XMStoreFloat4x4(&(constant_buffer_.projection), projection);
      }

      /**
       *  @brief  作成する
       *  @param  device:デバイス
       */
      void Create(Sein::Direct3D12::Device& device)
      {
        DirectX::XMStoreFloat4x4(&(constant_buffer_.view), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_.projection), DirectX::XMMatrixIdentity());

        constant_buffer_view_ = device.CreateConstantBuffer(sizeof(ConstantBufferType));
      }

    private:
      /**
       *  @brief  定数バッファ用構造体
       */
      struct ConstantBufferType
      {
        DirectX::XMFLOAT4X4 view;       ///< ビュー行列(視線空間)
        DirectX::XMFLOAT4X4 projection; ///< プロジェクション行列(射影空間)
      };
      ConstantBufferType constant_buffer_;                                      ///< 定数バッファ
      std::unique_ptr<Sein::Direct3D12::ConstantBuffer> constant_buffer_view_;  ///< 定数バッファビュー
    };
  };

  /**
   *  @brief  アプリケーション用シェーダーを作成する
   *  @param  device:デバイス
   *  @return アプリケーション用シェーダーインターフェイスへのポインタ
   */
  std::unique_ptr<IAppShader> IAppShader::Create(Sein::Direct3D12::Device& device)
  {
    auto app_shader = std::make_unique<AppShader>();
    app_shader->Create(device);

    return app_shader;
  }
};