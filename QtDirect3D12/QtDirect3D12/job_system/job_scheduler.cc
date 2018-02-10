/**
 *  @file     job_scheduler.cc
 *  @brief    ジョブスケジューラに関する処理を行うプログラムソース
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
    // 静的メンバ変数の定義
    std::unique_ptr<JobScheduler> JobScheduler::instance_ = std::make_unique<JobScheduler>();

    /**
     *  @brief  コンストラクタ
     */
    JobScheduler::JobScheduler()
    {
      assert(instance_ == nullptr);
    }
    
    /**
     *  @brief  デストラクタ
     */
    JobScheduler::~JobScheduler()
    {
      Destroy();
    }
    
    /**
     *  @brief  作成する
     *  @return 生成成功フラグ
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
     *  @brief  コンテナにジョブを登録する
     *  @param  job:コンテナに登録するジョブ
     *  @param  container:コンテナの種類
     */
    void JobScheduler::Register(Job* job, Containers container)
    {
      containers_[container].Register(job);
    }
    
    /**
     *  @brief  ジョブの登録を解除する
     *  @param  job:登録を解除するジョブへのポインタ
     */
    void JobScheduler::Unregister(Job* job)
    {
      // ジョブの登録を解除する
      for (decltype(auto) container : containers_)
      {
        container.Unregister(job);
      }
    }
    
    /**
     *  @brief  ジョブを実行する
     *  @param  delta_time:1フレームの経過時間
     */
    void JobScheduler::Execute(std::uint64_t delta_time)
    {
      // 各コンテナの実行準備
      for (decltype(auto) container : containers_)
      {
        container.MakeReady();
      }

      // 各コンテナのジョブを実行
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