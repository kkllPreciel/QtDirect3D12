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
    }
    
    /**
     *  @brief  ジョブを登録する
     *  @param  job:登録するジョブへのポインタ
     */
    void JobContainer::Register(Job* job)
    {
      job_list_.push_back(job);
      job->SetContainer(this);
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
  };
};