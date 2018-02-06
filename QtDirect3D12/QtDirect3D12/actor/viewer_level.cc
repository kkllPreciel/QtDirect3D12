/**
 *  @file     viewer_level.cc
 *  @brief    ビューア用レベルに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/06
 *  @version  1.0
 */

 // include
#include "actor/viewer_level.h"
#include "job_system/job_scheduler.h"
#include "job_system/async_job_manager.h"
#include "Loader/obj_loader.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  コンストラクタ
     */
    ViewerLevel::ViewerLevel()
    {

    }

    /**
     *  @brief  デストラクタ
     */
    ViewerLevel::~ViewerLevel()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool ViewerLevel::Create()
    {
      return true;
    }

    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void ViewerLevel::Execute(std::uint64_t delta_time)
    {
      if (initialized_ == false && async_job_.IsFinished())
      {
        initialized_ = true;
      }
    }

    /**
     *  @brief  終了処理を行う
     */
    void ViewerLevel::Destroy()
    {

    }

    /**
     *  @brief  モデルを読み込む
     *  @param  file_path:読み込むモデルのファイルパス
     */
    void ViewerLevel::LoadModel(std::string file_path)
    {
      Destroy();

      file_path_ = file_path;

      // job_.SetFunction([&](std::uint64_t delta_time) { this->Execute(delta_time); });
      // job_system::JobScheduler::GetInstance()->Register(&job_, job_system::JobScheduler::kMoveUpdate);

      // 非同期関数のジョブ登録
      async_job_.SetFunction([&]() { this->AsyncExecute(); });
      job_system::AsyncJobManager::GetInstance()->Push(&async_job_);
    }

    /**
     *  @brief  読み込み終了イベントを登録する
     *  @param  callback:読み込み終了時に実行する関数
     */
    void ViewerLevel::RegisterLoadedEvent(std::function<void(App::IModel*)> callback)
    {
      function_ = callback;
    }

    /**
     *  @brief  非同期実行処理関数
     */
    void ViewerLevel::AsyncExecute()
    {
      // 非同期読み込み処理

      auto loader = App::Loader::Obj::CreateLoader();
      auto model = loader->Load(file_path_, nullptr);
      function_(model.get());
    }
  };
};