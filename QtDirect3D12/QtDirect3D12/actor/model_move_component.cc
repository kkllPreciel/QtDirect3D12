/**
 *  @file     model_move_component.cc
 *  @brief    ���f���ړ������p�R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/08
 *  @version  1.0
 */

 // include
#include "actor/model_move_component.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �����t���R���X�g���N�^
     *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
     */
    ModelMoveComponent::ModelMoveComponent(Actor* owner) : Component(owner)
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    ModelMoveComponent::~ModelMoveComponent()
    {
      Destroy();
    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool ModelMoveComponent::Create()
    {
      Destroy();

      return true;
    }

    /**
     *  @brief  �I���������s��
     */
    void ModelMoveComponent::Destroy()
    {
      // �W���u�X�P�W���[���ւ̓o�^����������
      if (job_)
      {
        job_system::JobScheduler::GetInstance()->Unregister(job_.get());
      }

      job_.reset();
    }

    /**
     *  @brief  �X�V�������s��
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void ModelMoveComponent::Update(std::uint64_t delta_time)
    {
      auto axis = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, 1, 0));

      owner_->SetRotation(DirectX::XMQuaternionMultiply(owner_->GetRotation(), DirectX::XMQuaternionRotationRollPitchYaw(0, angle_, 0)));
    }
    
    /**
     *  @brief  1�t���[���ɉ�]����p�x(���W�A��)��ݒ肷��
     *  @param  angle:1�t���[���ɉ�]����p�x
     */
    void ModelMoveComponent::SetRotationAngle(const float angle)
    {
      angle_ = angle;
    }
  };
};