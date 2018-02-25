/**
 *  @file     mouse_driven_camera_move_component.cc
 *  @brief    �}�E�X�ő���ł���J�����ړ��p�R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/22
 *  @version  1.0
 */

 // include
#include "actor/mouse_driven_camera_move_component.h"
#include "actor/camera_component.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �����t���R���X�g���N�^
     *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
     */
    MouseDrivenCameraMoveComponent::MouseDrivenCameraMoveComponent(Actor* owner) : Component(owner)
    {

    }

    /**
     *  @brief  �f�X�g���N�^
     */
    MouseDrivenCameraMoveComponent::~MouseDrivenCameraMoveComponent()
    {
      Destroy();
    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool MouseDrivenCameraMoveComponent::Create()
    {
      clicked_ = false;

      Destroy();

      return true;
    }

    /**
     *  @brief  �I���������s��
     */
    void MouseDrivenCameraMoveComponent::Destroy()
    {
      // �W���u�X�P�W���[���ւ̓o�^����������
      if (job_)
      {
        job_system::JobScheduler::GetInstance()->Unregister(job_.get());
      }

      job_.reset();

      delta_ = 0;
    }

    /**
     *  @brief  �X�V�������s��
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void MouseDrivenCameraMoveComponent::Update(std::uint64_t delta_time)
    {
      delta_ += delta_time;
      auto time = static_cast<float>(delta_) / static_cast<float>(time_ * 60);

      DirectX::XMVECTOR now = DirectX::XMVectorLerp(begin_position_, end_position_, time);
      //now = DirectX::XMVectorAdd(DirectX::XMVectorScale(begin_position_, (1.0f - time_)), DirectX::XMVectorScale(end_position_, time_));
      owner_->SetPosition(now);

      // �J�����̃_�[�e�B(�X�V)�t���O�𗧂Ă�
      owner_->GetComponent<CameraComponent>()->SetDirty(true);

      // �ړ����I�������玩���I�ɍ폜����
      if (time_ * 60 <= delta_)
      {
        Destroy();
      }
    }

    /**
     *  @brief  �ړ����x��ݒ肷��
     *  @param  velocity:�ړ����x
     */
    void MouseDrivenCameraMoveComponent::SetVelocity(const float velocity)
    {
      velocity_ = velocity;
    }
    
    /**
     *  @brief  �ړ��W����ݒ肷��
     *  @param  coefficient:�ړ��W��
     */
    void MouseDrivenCameraMoveComponent::SetCoefficient(const float coefficient)
    {
      coefficient_ = coefficient;
    }

    /**
     *  @brief  �ړ��ɕK�v�Ȏ��Ԃ�ݒ肷��
     *  @param  time:�ړ��ɕK�v�Ȏ���
     */
    void MouseDrivenCameraMoveComponent::SetTime(std::uint64_t time)
    {
      time_ = time;
    }

    /**
     *  @brief  �����_�Ƃ̍Œ዗����ݒ肷��
     *  @param  distance:�����_�Ƃ̍Œ዗��
     */
    void MouseDrivenCameraMoveComponent::SetDistance(const float distance)
    {
      assert(1.0f <= distance);
      distance_ = distance;
    }
    
    /**
     *  @brief  �}�E�X�N���b�N�C�x���g����������
     *  @param  position:�}�E�X�̍��W
     */
    void MouseDrivenCameraMoveComponent::OnMousePressEvent(DirectX::XMVECTOR position)
    {
      Destroy();

      prev_mouse_position_ = position;
      clicked_ = true;
    }
    
    /**
     *  @brief  �}�E�X�����[�X�C�x���g����������
     *  @param  position:�}�E�X�̍��W
     */
    void MouseDrivenCameraMoveComponent::OnMouseReleaseEvent(DirectX::XMVECTOR position)
    {
      clicked_ = false;
    }
    
    /**
     *  @brief  �}�E�X�ړ��C�x���g����������
     *  @param  position:�}�E�X�̍��W
     */
    void MouseDrivenCameraMoveComponent::OnMouseMoveEvent(DirectX::XMVECTOR position)
    {
      if (clicked_ == false)
      {
        return;
      }

      // �J�������璍���_�ւ̃x�N�g���͕�����
      // �J�����̏�����x�N�g����������
      // �J�����̉������x�N�g�����v�Z�\
      // �}�E�X�̈ړ��x�N�g���ɉ����ď�����x�N�g���Ɖ������x�N�g�������Z����

      auto eye = owner_->GetPosition();

      auto component = owner_->GetComponent<CameraComponent>();
      auto at = component->GetLookAt();
      auto up = component->GetUpDirection();
      auto cross = DirectX::XMVector3Cross(up, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(at, eye)));

      auto value = DirectX::XMVectorSubtract(position, prev_mouse_position_);

      // �ړ������x�N�g��(���K���ς�)���쐬
      auto dir = DirectX::XMVector3Normalize(DirectX::XMVectorAdd(DirectX::XMVectorScale(up, -value.m128_f32[1]), DirectX::XMVectorScale(cross, value.m128_f32[0])));

      // ��]�����쐬
      auto rotation_axis = DirectX::XMQuaternionRotationAxis(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(at, eye)), dir), 0.1);

      // �J�����̉�]���s��
      owner_->SetPosition(DirectX::XMVector3Rotate(eye, rotation_axis));
      component->SetUpDirection(DirectX::XMVector3Rotate(up, rotation_axis));

      prev_mouse_position_ = position;

      // �J�����̃_�[�e�B(�X�V)�t���O�𗧂Ă�
      component->SetDirty(true);
    }
    
    /**
     *  @brief  �}�E�X�z�C�[���C�x���g����������
     *  @param  direction:�z�C�[���̉�]����
     */
    void MouseDrivenCameraMoveComponent::OnMouseWheelEvent(std::int32_t direction)
    {
      Destroy();

      if (clicked_)
      {
        return;
      }

      // ���_���璍���_�ւ̃x�N�g�����쐬
      DirectX::XMVECTOR eye = owner_->GetPosition();
      DirectX::XMVECTOR at = owner_->GetComponent<CameraComponent>()->GetLookAt();
      DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(at, eye);

      // ���_���璍���_�ւ̋������擾
      float distance = 0.0f;
      DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(dir));

      // ����������Ă���ꍇ�͈ړ��ʂɕ␳���s��
      const auto move = velocity_ * (std::fabsf(distance) * coefficient_);

      DirectX::XMVECTOR target = DirectX::XMVectorAdd(eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(dir), direction * move));

      // �����_�Ƃ̋��������ȉ��Ȃ�߂Â��Ȃ��悤�ɂ���
      dir = DirectX::XMVectorSubtract(at, target);
      distance = 0.0f;
      DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(dir));
      if (distance < distance_)
      {
        return;
      }

      begin_position_ = eye;
      end_position_ = target;

      // �W���u�ɓo�^����
      job_ = std::make_unique<job_system::Job>();

      job_->SetFunction([&](std::uint64_t delta_time) {
        this->Update(delta_time);
      });
      job_system::JobScheduler::GetInstance()->Register(job_.get(), job_system::JobScheduler::kMoveUpdate);
    }
  };
};