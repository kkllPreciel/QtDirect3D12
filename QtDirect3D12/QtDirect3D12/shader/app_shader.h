/**
 *  @file     app_shader.h
 *  @brief    �A�v���P�[�V�����Ŏg�p����V�F�[�_�[�Ɋւ���w�b�_�t�@�C��
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
   *  @brief  �A�v���P�[�V�����p�V�F�[�_�[�C���^�[�t�F�C�X
   */
  class IAppShader
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IAppShader() {};

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IAppShader() {};
    
    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IAppShader(const IAppShader& other) = delete;
    
    /**
     *  @brief  �R�s�[������Z�q�I�y���[�^
     *  @param  other:�R�s�[������̃C���X�^���X
     *  @return �R�s�[�����̃C���X�^���X
     */
    IAppShader& operator = (const IAppShader& other) = delete;

    /**
     *  @brief  �r���[�s���ݒ肷��
     *  @param  view:�r���[�s��
     */
    virtual void SetView(DirectX::XMMATRIX view) = 0;

    /**
     *  @brief  �v���W�F�N�V�����s���ݒ肷��
     *  @param  projection:�v���W�F�N�V�����s��
     */
    virtual void SetProjection(DirectX::XMMATRIX projection) = 0;

    /**
     *  @brief  �A�v���P�[�V�����p�V�F�[�_�[���쐬����
     *  @param  device:�f�o�C�X
     *  @return �A�v���P�[�V�����p�V�F�[�_�[�C���^�[�t�F�C�X�ւ̃|�C���^
     */
    static std::unique_ptr<IAppShader> Create(Sein::Direct3D12::Device& device);
  };
};