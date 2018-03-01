/**
 *  @file     job_scheduler.cc
 *  @brief    �W���u�X�P�W���[���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/17
 *  @version  1.0
 */

 // include
#include "job_system/job_scheduler.h"
#include <cassert>
#include "job_system/job.h"

namespace App
{
  namespace job_system
  {
    // �ÓI�����o�ϐ��̒�`
    std::unique_ptr<JobScheduler> JobScheduler::instance_ = std::make_unique<JobScheduler>();

    /**
     *  @brief  �R���X�g���N�^
     */
    JobScheduler::JobScheduler()
    {
      assert(instance_ == nullptr);
    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    JobScheduler::~JobScheduler()
    {
      Destroy();
    }

    /**
     *  @brief  �쐬����
     *  @param  num_threads:�X���b�h��
     *  @return ���������t���O
     */
    bool JobScheduler::Create(std::uint32_t num_threads)
    {
      assert(JobScheduler::Containers::kEnd < containers_.size());

      // �ړ������̓}���`�X���b�h�ŏ�������
      containers_[Containers::kMoveUpdate].EnableMulti();

      num_task_threads_ = num_threads;

      // TODO:0�Ԗڂ̈Ӗ���?
      threads_ = std::make_unique<JobThread[]>(num_task_threads_);
      for (std::uint32_t i = 0; i < num_task_threads_; ++i)
      {
        threads_[i].Create(i + 1, &queue_, true);
      }

      // �W���u�X���b�h���N������̂�҂�
      while (true)
      {
        if (queue_.WaitThreads() == num_task_threads_)
        {
          break;
        }
        else
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
      }

      return true;
    }
    
    void JobScheduler::Destroy()
    {
      queue_.Terminate();
      queue_.NotifyAll();
    }
    
    /**
     *  @brief  �R���e�i�ɃW���u��o�^����
     *  @param  job:�R���e�i�ɓo�^����W���u
     *  @param  container:�R���e�i�̎��
     */
    void JobScheduler::Register(Job* job, Containers container)
    {
      // TODO:�W���u���s���Ȃ�o�^��x�点��?(�ʂ̃L���[�ɐς�?)
      containers_[container].Register(job);
    }
    
    /**
     *  @brief  �W���u�̓o�^����������
     *  @param  job:�o�^����������W���u�ւ̃|�C���^
     */
    void JobScheduler::Unregister(Job* job)
    {
      job->GetContainer()->Unregister(job);
    }
    
    /**
     *  @brief  �W���u�����s����
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void JobScheduler::Execute(std::uint64_t delta_time)
    {
      // �e�R���e�i�̎��s����
      for (decltype(auto) container : containers_)
      {
        container.MakeReady();
      }

      queue_.Schedule(delta_time, containers_.data(), static_cast<std::uint32_t>(containers_.size()));

      Job* job = nullptr;
      while (true)
      {
        job = queue_.Pop(0);
        if (job)
        {
          job->Execute(delta_time);
          job->GetContainer()->NotifyFinished();
        }
        else
        {
          break;
        }
      }
    }
  };
};