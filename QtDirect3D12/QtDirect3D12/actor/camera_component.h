/**
 *  @file     camera_component.h
 *  @brief    �J�����p�R���|�[�l���g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/27
 *  @version  1.0
 */

#pragma once

 // include
#include <DirectXMath.h>
#include "actor/component.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �J�����p�̃R���|�[�l���g�N���X
     */
    class CameraComponent : public Component
    {
    public:
      /**
       *  @brief  �����t���R���X�g���N�^
       *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
       */
      CameraComponent(Actor* owner);

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~CameraComponent();

      /**
       *  @brief  �R���X�g���N�^
       */
      CameraComponent() = delete;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      CameraComponent(const CameraComponent& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      CameraComponent& operator = (const CameraComponent& other) = delete;

      /**
       *  @brief  �R���|�[�l���g�̎��ID���擾����
       *  @return �R���|�[�l���g�̎��ID
       */
      static constexpr std::uint32_t GetId()
      {
        // TODO:�������l�ɏC��
        return Component::ID::kMove;
      }

      /**
       *  @brief  ���������s��
       *  @return �����������t���O
       */
      virtual bool Create() override;

      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() override;

      /**
       *  @brief  �����_��ݒ肷��
       *  @param  look_at:�����_���W
       */
      void SetLookAt(DirectX::XMVECTOR look_at);

      /**
       *  @brief  �����_���擾����
       *  @return �����_���W
       */
      DirectX::XMVECTOR GetLookAt();

    private:
      DirectX::XMVECTOR look_at_;  ///< �����_���W
    };
  };
};