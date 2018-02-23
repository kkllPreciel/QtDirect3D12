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
#include "actor/model_render_component.h"
#include "actor/camera_component.h"
#include "actor/mouse_driven_camera_move_component.h"

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
      {
        auto camera_component = actors_[0].AddComponent<actor::CameraComponent>();
        camera_component->SetLookAt({ 0.0f, 10.0f, 0.0f });
        camera_component->SetUpDirection({ 0.0f, 1.0f, 0.0f });
        actors_[0].SetPosition({ 0.0f, 10.0f, -30.5f });

        // �}�E�X�ő���p�R���|�[�l���g���쐬
        auto component = actors_[0].AddComponent<actor::MouseDrivenCameraMoveComponent>();
        component->Create();
        component->SetTime(1);
        component->SetVelocity(1.0f);
        component->SetCoefficient(0.5f);
        component->SetDistance(1.0f);
      }

      // ���f���p�A�N�^�[�𐶐�����
      actors_[1].SetPosition({ 0.0f, 10.0f, 0.0f });

      // ���f���`��p�R���|�[�l���g���쐬
      decltype(auto) model_render_component = actors_[1].AddComponent<actor::ModelRenderComponent>();
      model_render_component->SetTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
      else if (async_job_.IsFinished())
      {
        // �X�V�������s��
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

      loaded_.disconnect_all_slots();
    }

    /**
     *  @brief  ���f����ǂݍ���
     *  @param  file_path:�ǂݍ��ރ��f���̃t�@�C���p�X
     */
    void ViewerLevel::LoadModel(std::string file_path)
    {
      file_path_ = file_path;

      // job_.SetFunction([&](std::uint64_t delta_time) { this->Execute(delta_time); });
      // job_system::JobScheduler::GetInstance()->Register(&job_, job_system::JobScheduler::kMoveUpdate);

      // �񓯊��֐��̃W���u�o�^
      async_job_.SetFunction([&]() { this->AsyncExecute(); });
      job_system::AsyncJobManager::GetInstance()->Push(&async_job_);
    }

    /**
     *  @brief  �ǂݍ��ݏI���V�O�i���C�x���g�ɃX���b�g(�֐�)��o�^����
     *  @param  callback:�ǂݍ��ݏI�����Ɏ��s����X���b�g(�֐�)
     */
    void ViewerLevel::RegisterLoadedSignalEvent(std::function<void(App::IAppModel*)> callback)
    {
      loaded_.connect(callback);
    }

    /**
     *  @brief  �}�E�X�N���b�N�C�x���g����������
     *  @param  position:�}�E�X�̍��W
     */
    void ViewerLevel::OnMousePressEvent(DirectX::XMVECTOR position)
    {
      // TODO:�I�u�T�[�o�[�p�^�[�����g�p���Ď�������
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMousePressEvent(position);
    }
    
    /**
     *  @brief  �}�E�X�����[�X�C�x���g����������
     *  @param  position:�}�E�X�̍��W
     */
    void ViewerLevel::OnMouseReleaseEvent(DirectX::XMVECTOR position)
    {
      // TODO:�I�u�T�[�o�[�p�^�[�����g�p���Ď�������
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMouseReleaseEvent(position);
    }
    
    /**
     *  @brief  �}�E�X�ړ��C�x���g����������
     *  @param  position:�}�E�X�̍��W
     */
    void ViewerLevel::OnMouseMoveEvent(DirectX::XMVECTOR position)
    {
      // TODO:�I�u�T�[�o�[�p�^�[�����g�p���Ď�������
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMouseMoveEvent(position);
    }
    
    /**
     *  @brief  �}�E�X�z�C�[���C�x���g����������
     *  @param  direction:�z�C�[���̉�]����
     */
    void ViewerLevel::OnMouseWheelEvent(std::int32_t direction)
    {
      // TODO:�I�u�T�[�o�[�p�^�[�����g�p���Ď�������
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMouseWheelEvent(direction);
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
     *  @brief  �f�o�C�X��ݒ肷��
     *  @param  device:�f�o�C�X
     */
    void ViewerLevel::SetDevice(std::unique_ptr<Sein::Direct3D12::Device> device)
    {
      device_.reset(nullptr);
      device_ = std::move(device);
    }

    /**
     *  @brief  �f�o�C�X���擾����
     *  @return �f�o�C�X
     */
    Sein::Direct3D12::Device& ViewerLevel::GetDevice()
    {
      return *device_;
    }

    /**
     *  @brief  �񓯊����s�����֐�
     */
    void ViewerLevel::AsyncExecute()
    {
      // �񓯊��ǂݍ��ݏ���

      auto model = App::IAppModel::LoadFromObj(*device_, file_path_);
      loaded_(model.get());
    }
  };
};