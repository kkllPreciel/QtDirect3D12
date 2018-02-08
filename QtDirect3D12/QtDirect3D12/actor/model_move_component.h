/**
 *  @file     model_move_component.h
 *  @brief    ���f���ړ������p�R���|�[�l���g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/02/08
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <DirectXMath.h>
#include "actor/component.h"
#include "job_system/job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  ���f���ړ������p�̃R���|�[�l���g�N���X
     */
    class ModelMoveComponent : public Component
    {
    public:
      /**
       *  @brief  �����t���R���X�g���N�^
       *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
       */
      ModelMoveComponent(Actor* owner);

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~ModelMoveComponent();

      /**
       *  @brief  �R���X�g���N�^
       */
      ModelMoveComponent() = delete;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      ModelMoveComponent(const ModelMoveComponent& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      ModelMoveComponent& operator = (const ModelMoveComponent& other) = delete;

      /**
       *  @brief  �R���|�[�l���g�̎��ID���擾����
       *  @return �R���|�[�l���g�̎��ID
       */
      static constexpr std::uint32_t GetId()
      {
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
       *  @brief  �X�V�������s��
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Update(std::uint64_t delta_time);

      /**
       *  @brief  1�t���[���ɉ�]����p�x(���W�A��)��ݒ肷��
       *  @param  angle:1�t���[���ɉ�]����p�x
       */
      void SetRotationAngle(const float angle);

    private:
      std::unique_ptr<job_system::Job> job_;  ///< �W���u
      float angle_;                           ///< 1�t���[���ɉ�]����p�x
    };
  };
};