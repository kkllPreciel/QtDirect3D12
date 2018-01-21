/**
 *  @file     move_component.h
 *  @brief    �ړ������p�R���|�[�l���g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/21
 *  @version  1.0
 */

#pragma once

 // include
#include <DirectXMath.h>
#include "actor/component.h"
#include "job_system/job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �ړ������p�̃R���|�[�l���g�N���X
     */
    class MoveComponent : public Component
    {
    public:
      /**
       *  @brief  �����t���R���X�g���N�^
       *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
       */
      MoveComponent(Actor* owner);

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~MoveComponent();

      /**
       *  @brief  �R���X�g���N�^
       */
      MoveComponent() = delete;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      MoveComponent(const MoveComponent& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      MoveComponent& operator = (const MoveComponent& other) = delete;

      /**
       *  @brief  �R���|�[�l���g�̎��ID���擾����
       *  @return �R���|�[�l���g�̎��ID
       */
      static constexpr Component::ID GetId();

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
       *  @brief  �X�V�������s��
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Update(std::uint64_t delta_time);

      /**
       *  @brief  �ړ�������ݒ肷��
       *  @param  velocity:�ړ�����
       */
      void SetVelocity(DirectX::XMVECTOR velocity);

      /**
       *  @brief  �ړ��������擾����
       *  @return �ړ�����
       */
      DirectX::XMVECTOR GetVelocity() const;

      /**
       *  @brief  ���x��ݒ肷��
       *  @param  speed:���x
       */
      void SetSpeed(float speed);

      /**
       *  @brief  ���x���擾����
       *  @return ���x
       */
      float GetSpeed() const;

    private:
      DirectX::XMVECTOR velocity_ = DirectX::XMVectorZero();  ///< �ړ�����
      float speed_ = 0.0f;                                    ///< ���x
    };
  };
};