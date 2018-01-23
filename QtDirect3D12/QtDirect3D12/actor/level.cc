/**
 *  @file     level.cc
 *  @brief    レベルに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/23
 *  @version  1.0
 */

 // include
#include "actor/level.h"
#include "job_system/job_scheduler.h"
#include "job_system/async_job_manager.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  コンストラクタ
     */
    Level::Level()
    {

    }

    /**
     *  @brief  デストラクタ
     */
    Level::~Level()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool Level::Create()
    {
      job_.SetFunction([&](std::uint64_t delta_time) { this->Execute(delta_time); });
      job_system::JobScheduler::GetInstance()->Register(&job_, job_system::JobScheduler::kMoveUpdate);

      async_job_.SetFunction([&]() { this->AsyncExecute(); });
      job_system::AsyncJobManager::GetInstance()->Push(&async_job_);

      return true;
      
    }

    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void Level::Execute(std::uint64_t delta_time)
    {
      if (initialized_ == false && async_job_.IsFinished())
      {
        initialized_ = true;
      }
    }

    /**
     *  @brief  終了処理を行う
     */
    void Level::Destroy()
    {

    }

    /**
     *  @brief  非同期実行処理関数
     */
    void Level::AsyncExecute()
    {
      // 非同期読み込み処理
    }
  };
};