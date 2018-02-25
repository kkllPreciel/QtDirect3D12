/**
 *  @file     camera_component.cc
 *  @brief    �J�����p�R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/27
 *  @version  1.0
 */

 // include
#include "actor/camera_component.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �����t���R���X�g���N�^
     *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
     */
    CameraComponent::CameraComponent(Actor* owner) : Component(owner)
    {

    }

    /**
     *  @brief  �f�X�g���N�^
     */
    CameraComponent::~CameraComponent()
    {
      Destroy();
    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool CameraComponent::Create()
    {
      dirty_ = true;

      return true;
    }

    /**
     *  @brief  �I���������s��
     */
    void CameraComponent::Destroy()
    {

    }

    /**
     *  @brief  �����_��ݒ肷��
     *  @param  look_at:�����_���W
     */
    void CameraComponent::SetLookAt(DirectX::XMVECTOR look_at)
    {
      look_at_ = look_at;
    }
    
    /**
     *  @brief  �����_���擾����
     *  @return �����_���W
     */
    DirectX::XMVECTOR CameraComponent::GetLookAt()
    {
      return look_at_;
    }

    /**
     *  @brief  �������ݒ肷��
     *  @param  up:�����
     */
    void CameraComponent::SetUpDirection(DirectX::XMVECTOR up)
    {
      up_ = up;
    }
    
    /**
     *  @brief  ��������擾����
     *  @return ������x�N�g��
     */
    DirectX::XMVECTOR CameraComponent::GetUpDirection()
    {
      return up_;
    }

    /**
     *  @brief  �r���[�s����擾����
     *  @return �r���[�s��
     */
    const DirectX::XMMATRIX& CameraComponent::GetViewMatrix()
    {
      // �_�[�e�B(�X�V)�t���O�������Ă��Ȃ�
      // �L���b�V�������s��Ŗ��Ȃ�
      if (false == dirty_)
      {
        return matrix_;
      }

      dirty_ = false;

      // �r���[�s����v�Z����
      DirectX::XMVECTOR eye = owner_->GetPosition();
      DirectX::XMVECTOR at = GetLookAt();;
      DirectX::XMVECTOR up = GetUpDirection();
      matrix_ = DirectX::XMMatrixLookAtLH(eye, at, up);

      return matrix_;
    }

    /**
     *  @brief  �_�[�e�B�t���O��ݒ肷��
     *  @param  dirty:�_�[�e�B�t���O
     */
    void CameraComponent::SetDirty(bool dirty)
    {
      dirty_ = dirty;
    }
  };
};