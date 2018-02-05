/**
 *  @file     camera_move_component.cc
 *  @brief    �J�����ړ������p�R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/22
 *  @version  1.0
 */

 // include
#include "actor/camera_move_component.h"
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
    CameraMoveComponent::CameraMoveComponent(Actor* owner) : Component(owner)
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    CameraMoveComponent::~CameraMoveComponent()
    {
      Destroy();
    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool CameraMoveComponent::Create()
    {
      Destroy();

      return true;
    }

    /**
     *  @brief  �I���������s��
     */
    void CameraMoveComponent::Destroy()
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
     *  @brief  ���΂���
     *  @param  direction:����
     */
    void CameraMoveComponent::Ignition(const float direction)
    {
      Destroy();

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

    /**
     *  @brief  �X�V�������s��
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void CameraMoveComponent::Update(std::uint64_t delta_time)
    {
      delta_ += delta_time;
      auto time = static_cast<float>(delta_) / static_cast<float>(time_ * 60);

      DirectX::XMVECTOR now = DirectX::XMVectorLerp(begin_position_, end_position_, time);
      //now = DirectX::XMVectorAdd(DirectX::XMVectorScale(begin_position_, (1.0f - time_)), DirectX::XMVectorScale(end_position_, time_));
      owner_->SetPosition(now);

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
    void CameraMoveComponent::SetVelocity(const float velocity)
    {
      velocity_ = velocity;
    }
    
    /**
     *  @brief  �ړ��W����ݒ肷��
     *  @param  coefficient:�ړ��W��
     */
    void CameraMoveComponent::SetCoefficient(const float coefficient)
    {
      coefficient_ = coefficient;
    }

    /**
     *  @brief  �ړ��ɕK�v�Ȏ��Ԃ�ݒ肷��
     *  @param  time:�ړ��ɕK�v�Ȏ���
     */
    void CameraMoveComponent::SetTime(std::uint64_t time)
    {
      time_ = time;
    }

    /**
     *  @brief  �����_�Ƃ̍Œ዗����ݒ肷��
     *  @param  distance:�����_�Ƃ̍Œ዗��
     */
    void CameraMoveComponent::SetDistance(const float distance)
    {
      assert(1.0f <= distance);
      distance_ = distance;
    }
  };
};