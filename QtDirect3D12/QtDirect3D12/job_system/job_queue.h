/**
 *  @file     job_queue.h
 *  @brief    �W���u�L���[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/02/28
 *  @version  1.0
 */

#pragma once

 // include
#include <mutex>
#include <atomic>
#include "job.h"
#include "job_container.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  �W���u�L���[�N���X
     */
    class JobQueue final
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      JobQueue();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~JobQueue();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      JobQueue(const JobQueue& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      JobQueue& operator = (const JobQueue& other) = delete;

      void Schedule(std::uint64_t delta_time, JobContainer* containers, std::uint32_t containers_size);

      Job* Pop(std::uint32_t thread_index);

      void NotifyAll();

      std::uint64_t delta_time() const;

      bool IsFinished() const;

      std::uint32_t WaitThreads() const;

      /**
       *  @brief  �I������
       */
      void Terminate();

    private:
      std::uint64_t delta_time_ = 0;
      JobContainer* containers_ = nullptr;

      std::atomic<std::uint32_t> current_index_ = 0;
      std::uint32_t final_index_ = 0;
      std::atomic<std::uint32_t> wait_threads_ = 0;

      std::mutex mutex_;
      std::condition_variable condition_;

      bool finished_ = false;
      bool terminate_ = false;
    };
  };
};