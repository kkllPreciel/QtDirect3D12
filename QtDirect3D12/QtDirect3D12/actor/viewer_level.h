/**
 *  @file     viewer_level.h
 *  @brief    �r���[�A�p���x���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/02/06
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include <array>
#include <boost/signals2/signal.hpp>
#include <Sein/Direct3D12/direct3d12_device.h>
#include "job_system/job.h"
#include "job_system/async_job.h"
#include "actor/actor.h"
#include "model/app_model.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �r���[�A�p���x���N���X
     */
    class ViewerLevel final
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      ViewerLevel();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~ViewerLevel();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      ViewerLevel(const ViewerLevel& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      ViewerLevel& operator = (const ViewerLevel& other) = delete;

      /**
       *  @brief  ���������s��
       *  @return �����������t���O
       */
      bool Create();

      /**
       *  @brief  �X�V�������s��
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time);

      /**
       *  @brief  �I���������s��
       */
      void Destroy();

      /**
       *  @brief  ���f����ǂݍ���
       *  @param  file_path:�ǂݍ��ރ��f���̃t�@�C���p�X
       */
      void LoadModel(std::string file_path);

      /**
       *  @brief  �ǂݍ��ݏI���V�O�i���C�x���g�ɃX���b�g(�֐�)��o�^����
       *  @param  callback:�ǂݍ��ݏI�����Ɏ��s����X���b�g(�֐�)
       */
      void RegisterLoadedSignalEvent(std::function<void(App::IAppModel*)> callback);

      /**
       *  @brief  �z�C�[���C�x���g�𔭍s����
       *  @param  force:�z�C�[���̉�]����
       */
      void DispatchWheelEvent(const std::float_t force);

      /**
       *  @brief  �J�����̍��W���擾����
       *  @return �J�����̍��W�x�N�g��
       */
      DirectX::XMVECTOR GetCameraPosition();

      /**
       *  @brief  �����_�̍��W���擾����
       *  @return �����_�̍��W�x�N�g��
       */
      DirectX::XMVECTOR GetLookAt();

      /**
       *  @brief  �f�o�C�X��ݒ肷��
       *  @param  device:�f�o�C�X
       */
      void SetDevice(std::unique_ptr<Sein::Direct3D12::Device> device);

      /**
       *  @brief  �f�o�C�X���擾����
       *  @return �f�o�C�X
       */
      Sein::Direct3D12::Device& GetDevice();

    private:
      /**
       *  @brief  �񓯊����s�����֐�
       */
      void AsyncExecute();

    private:
      std::unique_ptr<Sein::Direct3D12::Device> device_;      ///< �f�o�C�X
      std::array<actor::Actor, 10> actors_;                   ///< �A�N�^�[���X�g
      job_system::Job job_;                                   ///< �X�V�����p�W���u
      job_system::AsyncJob async_job_;                        ///< �񓯊������p�W���u
      bool initialized_ = false;                              ///< �������I���t���O
      std::string file_path_;                                 ///< ���f���t�@�C���p�X
      boost::signals2::signal<void(App::IAppModel*)> loaded_; ///< �ǂݍ��ݏI���V�O�i��
    };
  };
};