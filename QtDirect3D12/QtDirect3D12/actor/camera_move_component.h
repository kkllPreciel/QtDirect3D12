/**
 *  @file     camera_move_component.h
 *  @brief    �J�����ړ������p�R���|�[�l���g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/22
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
     *  @brief  �J�����ړ������p�̃R���|�[�l���g�N���X
     */
    class CameraMoveComponent : public Component
    {
    public:
      /**
       *  @brief  �����t���R���X�g���N�^
       *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
       */
      CameraMoveComponent(Actor* owner);

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~CameraMoveComponent();

      /**
       *  @brief  �R���X�g���N�^
       */
      CameraMoveComponent() = delete;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      CameraMoveComponent(const CameraMoveComponent& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      CameraMoveComponent& operator = (const CameraMoveComponent& other) = delete;

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
       *  @brief  �������W��ݒ肷��
       *  @param  begin_position:�������W
       */
      void SetBeginPosition(DirectX::XMVECTOR begin_position);

      /**
       *  @brief  �ڕW���W��ݒ肷��
       *  @param  end_position:�ڕW���W
       */
      void SetEndPosition(DirectX::XMVECTOR end_position);

      /**
       *  @brief  �ړ��ɕK�v�Ȏ��Ԃ�ݒ肷��
       *  @param  time:�ړ��ɕK�v�Ȏ���
       */
      void SetTime(std::uint64_t time);

    private:
      DirectX::XMVECTOR begin_position_;      ///< �������W
      DirectX::XMVECTOR end_position_;        ///< �ڕW���W
      std::uint64_t time_;                    ///< �ړ��ɕK�v�Ȏ���
      std::uint64_t delta_;                   ///< ���݂̌o�ߎ���
      std::unique_ptr<job_system::Job> job_;  ///< �W���u
    };
  };
};