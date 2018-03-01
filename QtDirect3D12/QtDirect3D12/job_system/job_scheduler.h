/**
 *  @file     job_scheduler.h
 *  @brief    ジョブスケジューラに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/17
 *  @version  1.0
 */

#pragma once

 // include
#include <array>
#include <memory>
#include "job_system/job_container.h"
#include "job_system/job_queue.h"
#include "job_system/job_thread.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  ジョブスケジューラクラス
     */
    class JobScheduler final
    {
    public:
      /**
       *  @brief  ジョブコンテナの種別
       */
      enum Containers : std::uint32_t
      {
        kIllegal = 0,
        kLoading,
        kMoveUpdate,
        kRenderUpdate,
        kEnd,
      };

      /**
       *  @brief  インスタンスを取得する
       *  @return ジョブスケジューラのポインタ
       */
      static std::unique_ptr<JobScheduler>& GetInstance()
      {
        return instance_;
      }

      /**
       *  @brief  コンストラクタ
       */
      JobScheduler();

      /**
       *  @brief  デストラクタ
       */
      ~JobScheduler();

      /**
       *  @brief  作成する
       *  @param  num_threads:スレッド数
       *  @return 生成成功フラグ
       */
      bool Create(std::uint32_t num_threads);

      void Destroy();

      /**
       *  @brief  コンテナにジョブを登録する
       *  @param  job:コンテナに登録するジョブ
       *  @param  container:コンテナの種類
       */
      void Register(Job* job, Containers container);

      /**
       *  @brief  ジョブの登録を解除する
       *  @param  job:登録を解除するジョブへのポインタ
       */
      void Unregister(Job* job);

      /**
       *  @brief  ジョブを実行する
       *  @param  delta_time:1フレームの経過時間
       */
      void Execute(std::uint64_t delta_time);

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      JobScheduler(const JobScheduler& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      JobScheduler& operator = (const JobScheduler& other) = delete;

    private:
      static std::unique_ptr<JobScheduler> instance_; ///< ジョブスケジューラのインスタンス
      std::array<JobContainer, 10> containers_;       ///< スケジュールするジョブコンテナリスト
      JobQueue queue_;                                ///< ジョブキュー
      std::unique_ptr<JobThread[]> threads_;          ///< ジョブスレッドリスト
      std::uint32_t num_task_threads_;                ///< タスクスレッド数
    };
  };
};