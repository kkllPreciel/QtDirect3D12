/**
 *  @file     job_container.cc
 *  @brief    ジョブコンテナに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/16
 *  @version  1.0
 */

 // include
#include "job_container.h"
#include <cassert>
#include "job_system/job.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  コンストラクタ
     */
    JobContainer::JobContainer()
    {
      // 128個分の領域を確保する
      job_list_.reserve(128);
    }
    
    /**
     *  @brief  デストラクタ
     */
    JobContainer::~JobContainer()
    {

    }

    /**
     *  @brief  実行の初期化を行う
     */
    void JobContainer::MakeReady()
    {
      current_index_ = 0;
      finished_count_ = 0;
      finished_ = job_list_.size() == 0;
    }
    
    /**
     *  @brief  ジョブを登録する
     *  @param  job:登録するジョブへのポインタ
     */
    void JobContainer::Register(Job* job)
    {
      job_list_.push_back(job);
      job->SetContainer(this);
      job_size_ = static_cast<std::uint32_t>(job_list_.size());
    }
    
    /**
     *  @brief  ジョブの登録を解除する
     *  @param  job:登録を解除するジョブへのポインタ
     */
    void JobContainer::Unregister(Job* job)
    {
      decltype(auto) end = job_list_.cend();
      for (decltype(auto) it = job_list_.begin(); it != end; ++it)
      {
        if (job == *it)
        {
          job->SetContainer(nullptr);
          job_list_.erase(it);
          break;
        }
      }

      job_size_ = static_cast<std::uint32_t>(job_list_.size());
    }
    
    /**
     *  @brief  実行するジョブを取得する
     *  @return 実行するジョブへのポインタ
     */
    Job* JobContainer::Pop()
    {
      std::uint32_t index = current_index_++;
      if (job_list_.size() <= index)
      {
        return nullptr;
      }

      return job_list_[index];
    }

    /**
     *  @brief  終了待ちを行う
     */
    void JobContainer::Wait()
    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (false == finished_)
      {
        condition_.wait(lock);
      }
    }

    /**
     *  @brief  終了通知を行う
     */
    void JobContainer::NotifyFinished()
    {
      if (++finished_count_ == job_size_)
      {
        std::unique_lock<std::mutex> lock(mutex_);
        finished_ = true;
        condition_.notify_one();
      }
    }
    
    /**
     *  @brief  処理が終了したか?
     *  @return 処理終了フラグ
     */
    bool JobContainer::IsFinished() const
    {
      return finished_;
    }
    
    /**
     *  @brief  マルチスレッドで実行するようにする
     */
    void JobContainer::EnableMulti()
    {
      multi_ = true;
    }
    
    /**
     *  @brief  マルチスレッドで実行するか?
     *  @return マルチスレッド実行フラグ
     */
    bool JobContainer::IsMulti() const
    {
      return multi_;
    }
  };
};