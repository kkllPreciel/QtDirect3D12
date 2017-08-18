#pragma once

#include <memory>
#include <QtWidgets/QWidget>
#include <DirectXMath.h>
#include "ui_QtDirect3D12.h"

namespace Sein {
  namespace Direct3D12 {
    class Device;
    class VertexBuffer;
    class IndexBuffer;
    class ConstantBuffer;
    class ShaderResourceBuffer;
  };
};

class QtDirect3D12 : public QWidget
{
  Q_OBJECT

public:
  QtDirect3D12(QWidget *parent = Q_NULLPTR);
  ~QtDirect3D12();

protected:
  void resizeEvent(QResizeEvent *event) Q_DECL_FINAL;

private Q_SLOTS:
  void mainLoop();

private:
  Ui::QtDirect3D12Class ui;
  std::unique_ptr<QTimer> timer;
  std::unique_ptr<Sein::Direct3D12::Device> device;
  std::unique_ptr<Sein::Direct3D12::VertexBuffer> vertexBuffer;
  std::unique_ptr<Sein::Direct3D12::IndexBuffer> indexBuffer;

  /**
   *  @brief  定数バッファ用構造体
   */
  struct ConstantBufferType
  {
    DirectX::XMFLOAT4X4 world;      ///< ワールド行列(世界空間)
    DirectX::XMFLOAT4X4 view;       ///< ビュー行列(視線空間)
    DirectX::XMFLOAT4X4 projection; ///< プロジェクション行列(射影空間)
  };
  ConstantBufferType constantBuffer;                                    ///< 定数バッファ
  std::unique_ptr<Sein::Direct3D12::ConstantBuffer> constantBufferView; ///< 定数バッファビュー

  /**
   *  @brief  インスタンシング用構造体
   */
  struct InstanceBuffer
  {
    DirectX::XMFLOAT4X4 world;  ///< ワールド行列(世界空間)
  };
  std::vector<InstanceBuffer> instanceBufferData;                               ///< 各インスタンス毎のデータリスト
  std::unique_ptr<Sein::Direct3D12::ShaderResourceBuffer> shaderResourceBuffer; ///< シェーダーリソースバッファビュー
  const unsigned int INSTANCE_NUM = 5;                                          ///< インスタンスの数

};
