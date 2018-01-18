/**
 *  @file     async_job_manager.cc
 *  @brief    �񓯊��W���u�Ǘ��Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/18
 *  @version  1.0
 */

 // include
#include "job_system/async_job_manager.h"
#include <cassert>
#include "job_system/async_job.h"

namespace App
{
  namespace job_system
  {
    // �ÓI�����o�ϐ��̒�`
    std::unique_ptr<AsyncJobManager> AsyncJobManager::instance_ = std::make_unique<AsyncJobManager>();

    /**
     *  @brief  �R���X�g���N�^
     */
    AsyncJobManager::AsyncJobManager() : threads_{ nullptr, nullptr, nullptr }, terminate_(false)
    {
      // �V���O���g���ۏ�
      assert(instance_ == nullptr);
    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    AsyncJobManager::~AsyncJobManager()
    {
      Destroy();
      assert(instance_);  // �V���O���g���ۏ�
    }

    /**
     *  @brief  ���������s��
     *  @return ���������t���O
     */
    bool AsyncJobManager::Create()
    {
      for (std::uint32_t i = 0; i < kNumThreads; ++i)
      {
        threads_[i] = new std::thread(AsyncJobManager::Run);
      }

      return true;
    }
    
    /**
     *  @brief  �I���������s��
     */
    void AsyncJobManager::Destroy()
    {
      terminate_ = true;
      condition_.notify_all();

      for (std::uint32_t i = 0; i < kNumThreads; ++i)
      {
        if (threads_[i] && threads_[i]->joinable())
        {
          threads_[i]->join();
          delete threads_[i];
          threads_[i] = nullptr;
        }
      }
    }
    
    /**
     *  @brief  �񓯊��W���u���L���[�֒ǉ�����
     *  @param  job:�L���[�֒ǉ�����񓯊��W���u
     *  @param  priority:���s�D��x
     */
    void AsyncJobManager::Push(AsyncJob* job, std::uint8_t priority)
    {
      std::unique_lock<std::mutex> lock(mutex_);

      job_queue_.insert(std::pair<std::uint8_t, AsyncJob*>(priority, job));
      condition_.notify_one();
    }
    
    /**
     *  @brief  ���s����񓯊��W���u���L���[������o��
     *  @return ���s����񓯊��W���u
     */
    AsyncJob* AsyncJobManager::Pop()
    {
      std::unique_lock<std::mutex> lock(mutex_);

      decltype(auto) it = job_queue_.begin();

      if (it != job_queue_.cend())
      {
        decltype(auto) job = it->second;
        job_queue_.erase(it);
        return job;
      }

      // ���s����W���u�����݂��Ȃ��̂őҋ@����
      condition_.wait(lock);

      return nullptr;
    }
    
    /**
     *  @brief  �X���b�h���s�����֐�
     */
    void AsyncJobManager::Run()
    {
      while (AsyncJobManager::instance_->terminate_ == false)
      {
        decltype(auto) job = AsyncJobManager::instance_->Pop();
        if (job)
        {
          job->Execute();
        }
      }
    }
  };
};