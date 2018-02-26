/**
 *  @file     app_shader.cc
 *  @brief    �A�v���P�[�V�����Ŏg�p����V�F�[�_�[�Ɋւ��鏈�����s���v���O�����\�[�X
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
     *  @param  �V�F�[�_�[�p�N���X
     */
    class AppShader final : public IAppShader
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      AppShader()
      {
    
      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~AppShader() override
      {

      }
    
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      AppShader(const AppShader& other) = delete;
    
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      AppShader& operator = (const AppShader& other) = delete;
    
      /**
       *  @brief  ���[�u�R���X�g���N�^
       *  @param  other:���[�u���̃C���X�^���X
       */
      AppShader(AppShader&& other) = delete;
    
      /**
       *  @brief  ���[�u������Z�q�I�y���[�^
       *  @param  other:���[�u������̃C���X�^���X
       *  @return ���[�u�����̃C���X�^���X
       */
      AppShader& operator = (AppShader&& other) = delete;

      /**
       *  @brief  �r���[�s���ݒ肷��
       *  @param  view:�r���[�s��
       */
      void SetView(DirectX::XMMATRIX view) override
      {
        DirectX::XMStoreFloat4x4(&(constant_buffer_.view), view);
      }
      
      /**
       *  @brief  �v���W�F�N�V�����s���ݒ肷��
       *  @param  projection:�v���W�F�N�V�����s��
       */
      void SetProjection(DirectX::XMMATRIX projection) override
      {
        DirectX::XMStoreFloat4x4(&(constant_buffer_.projection), projection);
      }

      /**
       *  @brief  �쐬����
       *  @param  device:�f�o�C�X
       */
      void Create(Sein::Direct3D12::Device& device)
      {
        DirectX::XMStoreFloat4x4(&(constant_buffer_.view), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_.projection), DirectX::XMMatrixIdentity());

        constant_buffer_view_ = device.CreateConstantBuffer(sizeof(ConstantBufferType));
      }

    private:
      /**
       *  @brief  �萔�o�b�t�@�p�\����
       */
      struct ConstantBufferType
      {
        DirectX::XMFLOAT4X4 view;       ///< �r���[�s��(�������)
        DirectX::XMFLOAT4X4 projection; ///< �v���W�F�N�V�����s��(�ˉe���)
      };
      ConstantBufferType constant_buffer_;                                      ///< �萔�o�b�t�@
      std::unique_ptr<Sein::Direct3D12::ConstantBuffer> constant_buffer_view_;  ///< �萔�o�b�t�@�r���[
    };
  };

  /**
   *  @brief  �A�v���P�[�V�����p�V�F�[�_�[���쐬����
   *  @param  device:�f�o�C�X
   *  @return �A�v���P�[�V�����p�V�F�[�_�[�C���^�[�t�F�C�X�ւ̃|�C���^
   */
  std::unique_ptr<IAppShader> IAppShader::Create(Sein::Direct3D12::Device& device)
  {
    auto app_shader = std::make_unique<AppShader>();
    app_shader->Create(device);

    return app_shader;
  }
};