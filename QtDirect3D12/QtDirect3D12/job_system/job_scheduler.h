/**
 *  @file     job_scheduler.h
 *  @brief    �W���u�X�P�W���[���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/17
 *  @version  1.0
 */

#pragma once

 // include
#include <array>
#include <memory>
#include "job_system/job_container.h"
#include "job_system/job_queue.h"
#include "job_system/job_thread.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  �W���u�X�P�W���[���N���X
     */
    class JobScheduler final
    {
    public:
      /**
       *  @brief  �W���u�R���e�i�̎��
       */
      enum Containers : std::uint32_t
      {
        kIllegal = 0,
        kLoading,
        kMoveUpdate,
        kRenderUpdate,
        kEnd,
      };

      /**
       *  @brief  �C���X�^���X���擾����
       *  @return �W���u�X�P�W���[���̃|�C���^
       */
      static std::unique_ptr<JobScheduler>& GetInstance()
      {
        return instance_;
      }

      /**
       *  @brief  �R���X�g���N�^
       */
      JobScheduler();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~JobScheduler();

      /**
       *  @brief  �쐬����
       *  @param  num_threads:�X���b�h��
       *  @return ���������t���O
       */
      bool Create(std::uint32_t num_threads);

      void Destroy();

      /**
       *  @brief  �R���e�i�ɃW���u��o�^����
       *  @param  job:�R���e�i�ɓo�^����W���u
       *  @param  container:�R���e�i�̎��
       */
      void Register(Job* job, Containers container);

      /**
       *  @brief  �W���u�̓o�^����������
       *  @param  job:�o�^����������W���u�ւ̃|�C���^
       */
      void Unregister(Job* job);

      /**
       *  @brief  �W���u�����s����
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time);

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      JobScheduler(const JobScheduler& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      JobScheduler& operator = (const JobScheduler& other) = delete;

    private:
      static std::unique_ptr<JobScheduler> instance_; ///< �W���u�X�P�W���[���̃C���X�^���X
      std::array<JobContainer, 10> containers_;       ///< �X�P�W���[������W���u�R���e�i���X�g
      JobQueue queue_;                                ///< �W���u�L���[
      std::unique_ptr<JobThread[]> threads_;          ///< �W���u�X���b�h���X�g
      std::uint32_t num_task_threads_;                ///< �^�X�N�X���b�h��
    };
  };
};