/**
 *  @file     camera_move_component.cc
 *  @brief    �J�����ړ������p�R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/22
 *  @version  1.0
 */

 // include
#include "actor/camera_move_component.h"
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

    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool CameraMoveComponent::Create()
    {
      Destroy();

      job_ = std::make_unique<job_system::Job>();

      job_->SetFunction([&](std::uint64_t delta_time) {
        this->Update(delta_time);
      });
      job_system::JobScheduler::GetInstance()->Register(job_.get(), job_system::JobScheduler::kMoveUpdate);

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
     *  @brief  �������W��ݒ肷��
     *  @param  begin_position:�������W
     */
    void CameraMoveComponent::SetBeginPosition(DirectX::XMVECTOR begin_position)
    {
      begin_position_ = begin_position;
    }

    /**
     *  @brief  �ڕW���W��ݒ肷��
     *  @param  end_position:�ڕW���W
     */
    void CameraMoveComponent::SetEndPosition(DirectX::XMVECTOR end_position)
    {
      end_position_ = end_position;
    }

    /**
     *  @brief  �ړ��ɕK�v�Ȏ��Ԃ�ݒ肷��
     *  @param  time:�ړ��ɕK�v�Ȏ���
     */
    void CameraMoveComponent::SetTime(std::uint64_t time)
    {
      time_ = time;
    }
  };
};