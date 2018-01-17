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
#include "job_system/job_container.h"

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
        kCameraUpdate,
        kRenderUpdate,
        kEnd,
      };

      /**
       *  @brief  インスタンスを取得する
       *  @return ジョブスケジューラのポインタ
       */
      static JobScheduler* GetInstance()
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
       *  @return 生成成功フラグ
       */
      bool Create();

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
      static JobScheduler* instance_;           ///< ジョブスケジューラのインスタンス
      std::array<JobContainer, 10> containers_; ///< スケジュールするジョブコンテナリスト
    };
  };
};