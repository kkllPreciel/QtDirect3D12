/**
 *  @file     mouse_driven_camera_move_component.h
 *  @brief    �}�E�X�ő���ł���J�����ړ��p�R���|�[�l���g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/02/22
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
     *  @brief  �}�E�X�ő���ł���J�����ړ��p�̃R���|�[�l���g�N���X
     */
    class MouseDrivenCameraMoveComponent : public Component
    {
    public:
      /**
       *  @brief  �����t���R���X�g���N�^
       *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
       */
      MouseDrivenCameraMoveComponent(Actor* owner);

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~MouseDrivenCameraMoveComponent();

      /**
       *  @brief  �R���X�g���N�^
       */
      MouseDrivenCameraMoveComponent() = delete;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      MouseDrivenCameraMoveComponent(const MouseDrivenCameraMoveComponent& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      MouseDrivenCameraMoveComponent& operator = (const MouseDrivenCameraMoveComponent& other) = delete;

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
       *  @brief  �ړ����x��ݒ肷��
       *  @param  velocity:�ړ����x
       */
      void SetVelocity(const float velocity);

      /**
       *  @brief  �ړ��W����ݒ肷��
       *  @param  coefficient:�ړ��W��
       */
      void SetCoefficient(const float coefficient);

      /**
       *  @brief  �����_�Ƃ̍Œ዗����ݒ肷��
       *  @param  distance:�����_�Ƃ̍Œ዗��
       */
      void SetDistance(const float distance);

      /**
       *  @brief  �ړ��ɕK�v�Ȏ��Ԃ�ݒ肷��
       *  @param  time:�ړ��ɕK�v�Ȏ���
       */
      void SetTime(std::uint64_t time);

      /**
       *  @brief  �}�E�X�N���b�N�C�x���g����������
       *  @param  position:�}�E�X�̍��W
       */
      void OnMousePressEvent(DirectX::XMVECTOR position);

      /**
       *  @brief  �}�E�X�����[�X�C�x���g����������
       *  @param  position:�}�E�X�̍��W
       */
      void OnMouseReleaseEvent(DirectX::XMVECTOR position);

      /**
       *  @brief  �}�E�X�ړ��C�x���g����������
       *  @param  position:�}�E�X�̍��W
       */
      void OnMouseMoveEvent(DirectX::XMVECTOR position);

      /**
       *  @brief  �}�E�X�z�C�[���C�x���g����������
       *  @param  direction:�z�C�[���̉�]����
       */
      void OnMouseWheelEvent(std::int32_t direction);

    private:
      std::unique_ptr<job_system::Job> job_;  ///< �W���u
      float velocity_;                        ///< �ړ����x
      float coefficient_;                     ///< �ړ��W��
      float distance_;                        ///< �����_�Ƃ̍Œ዗��
      DirectX::XMVECTOR begin_position_;      ///< �������W
      DirectX::XMVECTOR end_position_;        ///< �ڕW���W
      std::uint64_t time_;                    ///< �ړ��ɕK�v�Ȏ���
      std::uint64_t delta_;                   ///< ���݂̌o�ߎ���
      DirectX::XMVECTOR prev_mouse_position_; ///< 1�O�̃}�E�X�̍��W
      bool clicked_;                          ///< �N���b�N�t���O
    };
  };
};