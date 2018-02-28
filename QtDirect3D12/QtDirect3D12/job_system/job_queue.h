/**
 *  @file     job_queue.h
 *  @brief    ジョブキューに関するヘッダファイル
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
     *  @brief  ジョブキュークラス
     */
    class JobQueue final
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      JobQueue();

      /**
       *  @brief  デストラクタ
       */
      ~JobQueue();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      JobQueue(const JobQueue& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      JobQueue& operator = (const JobQueue& other) = delete;

      void Schedule(std::uint64_t delta_time, JobContainer* containers, std::uint32_t containers_size);

      Job* Pop(std::uint32_t thread_index);

      void NotifyAll();

      std::uint64_t delta_time() const;

      bool IsFinished() const;

      std::uint32_t WaitThreads() const;

      /**
       *  @brief  終了する
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