/**
 *  @file     async_job_manager.cc
 *  @brief    非同期ジョブ管理に関する処理を行うプログラムソース
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
    // 静的メンバ変数の定義
    std::unique_ptr<AsyncJobManager> AsyncJobManager::instance_ = std::make_unique<AsyncJobManager>();

    /**
     *  @brief  コンストラクタ
     */
    AsyncJobManager::AsyncJobManager() : threads_{ nullptr, nullptr, nullptr }, terminate_(false)
    {
      // シングルトン保証
      assert(instance_ == nullptr);
    }
    
    /**
     *  @brief  デストラクタ
     */
    AsyncJobManager::~AsyncJobManager()
    {
      Destroy();
      assert(instance_);  // シングルトン保証
    }

    /**
     *  @brief  初期化を行う
     *  @return 生成成功フラグ
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
     *  @brief  終了処理を行う
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
     *  @brief  非同期ジョブをキューへ追加する
     *  @param  job:キューへ追加する非同期ジョブ
     *  @param  priority:実行優先度
     */
    void AsyncJobManager::Push(AsyncJob* job, std::uint8_t priority)
    {
      std::unique_lock<std::mutex> lock(mutex_);

      job_queue_.insert(std::pair<std::uint8_t, AsyncJob*>(priority, job));
      condition_.notify_one();
    }
    
    /**
     *  @brief  実行する非同期ジョブをキューから取り出す
     *  @return 実行する非同期ジョブ
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

      // 実行するジョブが存在しないので待機する
      condition_.wait(lock);

      return nullptr;
    }
    
    /**
     *  @brief  スレッド実行される関数
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