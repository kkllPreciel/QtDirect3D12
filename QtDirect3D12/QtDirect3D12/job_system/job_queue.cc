/**
 *  @file     job_queue.cc
 *  @brief    �W���u�L���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/28
 *  @version  1.0
 */

 // include
#include "job_queue.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    JobQueue::JobQueue()
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    JobQueue::~JobQueue()
    {

    }
    
    void JobQueue::Schedule(std::uint64_t delta_time, JobContainer* containers, std::uint32_t containers_size)
    {
      delta_time_ = delta_time;
      containers_ = containers;
      current_index_ = 0;
      final_index_ = containers_size;
      finished_ = false;
    }
    
    Job* JobQueue::Pop(std::uint32_t thread_index)
    {
      while (true)
      {
        if (terminate_)
        {
          break;
        }

        std::uint32_t index = current_index_;
        if (index == final_index_)
        {
          if (thread_index == 0)
          {
            containers_[index - 1].Wait();
            finished_ = true;
            break;
          }
          else
          {
            std::unique_lock<std::mutex> lock(mutex_);
            ++wait_threads_;
            condition_.wait(lock);
            --wait_threads_;
            continue;
          }
        }

        JobContainer& container = containers_[index];
        Job* job = nullptr;

        // �}���`�X���b�h�p�W���u�R���e�i�Ȃ�
        // �ǂ̃X���b�h�ł�Pop�\
        if (container.IsMulti())
        {
          job = container.Pop();
        }
        // �V���O���X���b�h�p�Ȃ烁�C���X���b�h�̂�Pop�\
        else if (thread_index == 0)
        {
          job = container.Pop();
        }

        if (job)
        {
          return job;
        }
        else
        {
          if (thread_index == 0)
          {
            container.Wait();
            ++current_index_;
            NotifyAll();
          }
          else
          {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait(lock);
          }
        }
      }

      return nullptr;
    }
    
    void JobQueue::NotifyAll()
    {
      condition_.notify_all();
    }
    
    std::uint64_t JobQueue::delta_time() const
    {
      return delta_time_;
    }
    
    bool JobQueue::IsFinished() const
    {
      return finished_;
    }
    
    std::uint32_t JobQueue::WaitThreads() const
    {
      return wait_threads_;
    }
    
    /**
     *  @brief  �I������
     */
    void JobQueue::Terminate()
    {
      terminate_ = true;
    }
  };
};