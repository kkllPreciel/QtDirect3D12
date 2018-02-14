/**
 *  @file     model_render_component.cc
 *  @brief    ���f���`�揈���p�R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/14
 *  @version  1.0
 */

 // include
#include "actor/model_render_component.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �����t���R���X�g���N�^
     *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
     */
    ModelRenderComponent::ModelRenderComponent(Actor* owner) : Component(owner)
    {

    }

    /**
     *  @brief  �f�X�g���N�^
     */
    ModelRenderComponent::~ModelRenderComponent()
    {
      Destroy();
    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool ModelRenderComponent::Create()
    {
      Destroy();

      // �W���u�ɓo�^����
      job_ = std::make_unique<job_system::Job>();

      job_->SetFunction([&](std::uint64_t delta_time) {
        this->Update(delta_time);
      });
      job_system::JobScheduler::GetInstance()->Register(job_.get(), job_system::JobScheduler::kRenderUpdate);

      return true;
    }
    
    /**
     *  @brief  �I���������s��
     */
    void ModelRenderComponent::Destroy()
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
    void ModelRenderComponent::Update(std::uint64_t delta_time)
    {
      // �`�悷��Ȃ�s����X�V����
      // �I�[�i�[�̍��W�A�����A�X�P�[�����O����s��𐶐�����
      // ���f���ƕK�v�ȏ��������_�����O�L���[�ɓo�^����
    }
    
    /**
     *  @brief  ���f����ݒ肷��
     *  @param  model:���f��
     */
    void ModelRenderComponent::SetModel(std::shared_ptr<App::IModel> model)
    {
      model_ = model;
    }
  };
};