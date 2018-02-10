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
     *  @return ���������t���O
     */
    bool JobScheduler::Create()
    {
      assert(JobScheduler::Containers::kEnd < containers_.size());

      return true;
    }
    
    void JobScheduler::Destroy()
    {

    }
    
    /**
     *  @brief  �R���e�i�ɃW���u��o�^����
     *  @param  job:�R���e�i�ɓo�^����W���u
     *  @param  container:�R���e�i�̎��
     */
    void JobScheduler::Register(Job* job, Containers container)
    {
      containers_[container].Register(job);
    }
    
    /**
     *  @brief  �W���u�̓o�^����������
     *  @param  job:�o�^����������W���u�ւ̃|�C���^
     */
    void JobScheduler::Unregister(Job* job)
    {
      // �W���u�̓o�^����������
      for (decltype(auto) container : containers_)
      {
        container.Unregister(job);
      }
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

      // �e�R���e�i�̃W���u�����s
      for (decltype(auto) container : containers_)
      {
        Job* job = nullptr;
        while ((job = container.Pop()) != nullptr)
        {
          job->Execute(delta_time);
        }
      }
    }
  };
};