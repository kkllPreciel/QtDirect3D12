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
     *  @param  num_threads:スレッド数
     *  @return 生成成功フラグ
     */
    bool JobScheduler::Create(std::uint32_t num_threads)
    {
      assert(JobScheduler::Containers::kEnd < containers_.size());

      // 移動処理はマルチスレッドで処理する
      containers_[Containers::kMoveUpdate].EnableMulti();

      num_task_threads_ = num_threads;

      // TODO:0番目の意味は?
      threads_ = std::make_unique<JobThread[]>(num_task_threads_);
      for (std::uint32_t i = 0; i < num_task_threads_; ++i)
      {
        threads_[i].Create(i + 1, &queue_, true);
      }

      // ジョブスレッドが起動するのを待つ
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
     *  @brief  コンテナにジョブを登録する
     *  @param  job:コンテナに登録するジョブ
     *  @param  container:コンテナの種類
     */
    void JobScheduler::Register(Job* job, Containers container)
    {
      // 追加するジョブのリスト
       test_[update_.size()] = container;
       update_.emplace_back(job);

      // 更新するジョブのリスト(コンテナ番号を保持していたら追加)
      // 登録 -> 削除
      // 削除 -> 登録
      // 登録(問題なし)
      // 削除(問題なし)
      // TODO:ジョブ実行中なら登録を遅らせる?(別のキューに積む?)
      // containers_[container].Register(job);
    }
    
    /**
     *  @brief  ジョブの登録を解除する
     *  @param  job:登録を解除するジョブへのポインタ
     */
    void JobScheduler::Unregister(Job* job)
    {
      // 削除するジョブのリスト
      test_[update_.size()] = Containers::kIllegal;
      update_.emplace_back(job);

      // job->GetContainer()->Unregister(job);
    }
    
    /**
     *  @brief  ジョブを実行する
     *  @param  delta_time:1フレームの経過時間
     */
    void JobScheduler::Execute(std::uint64_t delta_time)
    {
      // 各コンテナにジョブを積む
      for (auto i = 0; i < update_.size(); ++i)
      {
        decltype(auto) job = update_[i];
        decltype(auto) container = test_[i];

        if (container == Containers::kIllegal)
        {
          job->GetContainer()->Unregister(job);
        }
        else
        {
          containers_[container].Register(job);
        }
      }
      update_.clear();

      // 各コンテナの実行準備
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