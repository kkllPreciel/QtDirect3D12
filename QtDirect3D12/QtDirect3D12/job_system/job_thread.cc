/**
 *  @file     job_thread.cc
 *  @brief    ジョブスレッドに関する処理を行うプログラムソース
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
     *  @brief  コンストラクタ
     */
    JobThread::JobThread()
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    JobThread::~JobThread()
    {
      Destroy();
    }
    
    /**
     *  @brief  作成する
     *  @param  thread_index:ユニークID
     *  @param  queue:ジョブキュー
     *  @param  run:実行フラグ
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
     *  @brief  終了処理を行う
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
     *  @brief  ジョブ実行関数
     *  @param  job:実行するジョブ
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
     *  @brief  スレッド実行関数
     *  @param  thread_index:ユニークID
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