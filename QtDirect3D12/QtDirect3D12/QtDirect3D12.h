#pragma once

#include <memory>
#include <QtWidgets/QWidget>
#include <d3d12.h>
#include <DirectXMath.h>
#include "ui_QtDirect3D12.h"
#include "actor/actor.h"
#include "actor/viewer_level.h"

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
  void dragEnterEvent(QDragEnterEvent *event) Q_DECL_FINAL;
  void dropEvent(QDropEvent *event) Q_DECL_FINAL;
  void wheelEvent(QWheelEvent *event) Q_DECL_FINAL;
  void keyPressEvent(QKeyEvent *event) Q_DECL_FINAL;

private Q_SLOTS:
  void mainLoop();

private:
  Ui::QtDirect3D12Class ui;
  std::unique_ptr<QTimer> timer;
  std::unique_ptr<Sein::Direct3D12::Device> device;
  std::unique_ptr<Sein::Direct3D12::VertexBuffer> vertexBuffer;
  std::unique_ptr<Sein::Direct3D12::IndexBuffer> indexBuffer;
  std::uint64_t index_count_;
  std::unique_ptr<App::actor::Actor> camera_;       ///< カメラアクター
  D3D12_PRIMITIVE_TOPOLOGY topology_;               ///< プリミティブのタイプ
  std::unique_ptr<App::actor::ViewerLevel> level_;  ///< ビューア用レベル
  bool is_loading = false;                          ///< 読み込み中フラグ

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
