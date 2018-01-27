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
  };
};