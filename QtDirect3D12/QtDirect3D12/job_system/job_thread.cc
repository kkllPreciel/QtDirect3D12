/**
 *  @file     job_thread.cc
 *  @brief    �W���u�X���b�h�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/27
 *  @version  1.0
 */

 // include
#include "job_thread.h"
#include "job.h"
#include "job_container.h"
#include "job_queue.h"

namespace App
{
  namespace job_system
  {
    std::uint32_t JobThread::thread_index_ = 0;

    /**
     *  @brief  �R���X�g���N�^
     */
    JobThread::JobThread()
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    JobThread::~JobThread()
    {
      Destroy();
    }
    
    /**
     *  @brief  �쐬����
     *  @param  thread_index:���j�[�NID
     *  @param  queue:�W���u�L���[
     *  @param  run:���s�t���O
     */
    void JobThread::Create(std::uint32_t thread_index, JobQueue* queue, bool run)
    {
      queue_ = queue;

      if (run)
      {
        thread_ = new std::thread(&JobThread::Run, this, thread_index);
      }
    }
    
    /**
     *  @brief  �I���������s��
     */
    void JobThread::Destroy()
    {
      if (thread_ && thread_->joinable())
      {
        thread_->join();
        delete thread_;
        thread_ = nullptr;
      }
    }
    
    /**
     *  @brief  �W���u���s�֐�
     *  @param  job:���s����W���u
     */
    void JobThread::Execute(Job* job)
    {
      current_ = job;
      job->Execute(queue_->delta_time());
      job->GetContainer()->NotifyFinished();
      current_ = nullptr;
    }
    
    void JobThread::SetThreadIndex(std::uint32_t index)
    {
      thread_index_ = index;
    }
    
    std::uint32_t JobThread::GetThreadIndex()
    {
      return thread_index_;
    }
    
    /**
     *  @brief  �X���b�h���s�֐�
     *  @param  thread_index:���j�[�NID
     */
    void JobThread::Run(std::uint32_t thread_index)
    {
      SetThreadIndex(thread_index);

      Job* job = nullptr;
      while ((job = queue_->Pop(thread_index)) != nullptr)
      {
        Execute(job);
      }
    }
  };
};