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
  std::unique_ptr<App::actor::Actor> camera_;       ///< �J�����A�N�^�[
  D3D12_PRIMITIVE_TOPOLOGY topology_;               ///< �v���~�e�B�u�̃^�C�v
  std::unique_ptr<App::actor::ViewerLevel> level_;  ///< �r���[�A�p���x��
  bool is_loading = false;                          ///< �ǂݍ��ݒ��t���O

  /**
   *  @brief  �萔�o�b�t�@�p�\����
   */
  struct ConstantBufferType
  {
    DirectX::XMFLOAT4X4 world;      ///< ���[���h�s��(���E���)
    DirectX::XMFLOAT4X4 view;       ///< �r���[�s��(�������)
    DirectX::XMFLOAT4X4 projection; ///< �v���W�F�N�V�����s��(�ˉe���)
  };
  ConstantBufferType constantBuffer;                                    ///< �萔�o�b�t�@
  std::unique_ptr<Sein::Direct3D12::ConstantBuffer> constantBufferView; ///< �萔�o�b�t�@�r���[

  /**
   *  @brief  �C���X�^���V���O�p�\����
   */
  struct InstanceBuffer
  {
    DirectX::XMFLOAT4X4 world;  ///< ���[���h�s��(���E���)
  };
  std::vector<InstanceBuffer> instanceBufferData;                               ///< �e�C���X�^���X���̃f�[�^���X�g
  std::unique_ptr<Sein::Direct3D12::ShaderResourceBuffer> shaderResourceBuffer; ///< �V�F�[�_�[���\�[�X�o�b�t�@�r���[
  const unsigned int INSTANCE_NUM = 5;                                          ///< �C���X�^���X�̐�

};
