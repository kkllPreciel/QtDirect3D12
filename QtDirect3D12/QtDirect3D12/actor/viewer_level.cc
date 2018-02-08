/**
 *  @file     viewer_level.cc
 *  @brief    �r���[�A�p���x���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/06
 *  @version  1.0
 */

 // include
#include "actor/viewer_level.h"
#include "job_system/job_scheduler.h"
#include "job_system/async_job_manager.h"
#include "actor/camera_component.h"
#include "actor/camera_move_component.h"
#include "Loader/obj_loader.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    ViewerLevel::ViewerLevel()
    {

    }

    /**
     *  @brief  �f�X�g���N�^
     */
    ViewerLevel::~ViewerLevel()
    {
      Destroy();
    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool ViewerLevel::Create()
    {
      // �A�N�^�[�𐶐�����(TODO:�ݒ�t�@�C������ǂݍ���)
      actors_[0].AddComponent<actor::CameraComponent>()->SetLookAt({ 0.0f, 10.0f, 0.0f });
      actors_[0].SetPosition({ 0.0f, 10.0f, -30.5f });

      // �ړ��p�R���|�[�l���g���쐬
      auto component = actors_[0].AddComponent<actor::CameraMoveComponent>();
      component->Create();
      component->SetTime(1);
      component->SetVelocity(1.0f);
      component->SetCoefficient(0.5f);
      component->SetDistance(1.0f);

      return true;
    }

    /**
     *  @brief  �X�V�������s��
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void ViewerLevel::Execute(std::uint64_t delta_time)
    {
      if (initialized_ == false && async_job_.IsFinished())
      {
        initialized_ = true;
      }
    }

    /**
     *  @brief  �I���������s��
     */
    void ViewerLevel::Destroy()
    {
      for (auto& actor : actors_)
      {
        actor.Destroy();
      }
    }

    /**
     *  @brief  ���f����ǂݍ���
     *  @param  file_path:�ǂݍ��ރ��f���̃t�@�C���p�X
     */
    void ViewerLevel::LoadModel(std::string file_path)
    {
      Destroy();

      file_path_ = file_path;

      // job_.SetFunction([&](std::uint64_t delta_time) { this->Execute(delta_time); });
      // job_system::JobScheduler::GetInstance()->Register(&job_, job_system::JobScheduler::kMoveUpdate);

      // �񓯊��֐��̃W���u�o�^
      async_job_.SetFunction([&]() { this->AsyncExecute(); });
      job_system::AsyncJobManager::GetInstance()->Push(&async_job_);
    }

    /**
     *  @brief  �ǂݍ��ݏI���C�x���g��o�^����
     *  @param  callback:�ǂݍ��ݏI�����Ɏ��s����֐�
     */
    void ViewerLevel::RegisterLoadedEvent(std::function<void(IModel*)> callback)
    {
      function_ = callback;
    }

    /**
     *  @brief  �z�C�[���C�x���g�𔭍s����
     *  @param  force:�z�C�[���̉�]����
     */
    void ViewerLevel::DispatchWheelEvent(const std::float_t force)
    {
      // TODO:�I�u�T�[�o�[�p�^�[�����g�p���Ď�������
      actors_[0].GetComponent<actor::CameraMoveComponent>()->Ignition(force);
    }

    /**
     *  @brief  �J�����̍��W���擾����
     *  @return �J�����̍��W�x�N�g��
     */
    DirectX::XMVECTOR ViewerLevel::GetCameraPosition()
    {
      return actors_[0].GetPosition();
    }
    
    /**
     *  @brief  �����_�̍��W���擾����
     *  @return �����_�̍��W�x�N�g��
     */
    DirectX::XMVECTOR ViewerLevel::GetLookAt()
    {
      return actors_[0].GetComponent<actor::CameraComponent>()->GetLookAt();
    }

    /**
     *  @brief  �񓯊����s�����֐�
     */
    void ViewerLevel::AsyncExecute()
    {
      // �񓯊��ǂݍ��ݏ���

      auto loader = App::Loader::Obj::CreateLoader();
      auto model = loader->Load(file_path_, nullptr);
      function_(model.get());
    }
  };
};