/**
 *  @file     viewer_level.h
 *  @brief    ビューア用レベルに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/06
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include <array>
#include "job_system/job.h"
#include "job_system/async_job.h"
#include "actor/actor.h"
#include "Loader/model.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  ビューア用レベルクラス
     */
    class ViewerLevel final
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      ViewerLevel();

      /**
       *  @brief  デストラクタ
       */
      ~ViewerLevel();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      ViewerLevel(const ViewerLevel& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      ViewerLevel& operator = (const ViewerLevel& other) = delete;

      /**
       *  @brief  初期化を行う
       *  @return 初期化成功フラグ
       */
      bool Create();

      /**
       *  @brief  更新処理を行う
       *  @param  delta_time:1フレームの経過時間
       */
      void Execute(std::uint64_t delta_time);

      /**
       *  @brief  終了処理を行う
       */
      void Destroy();

      /**
       *  @brief  モデルを読み込む
       *  @param  file_path:読み込むモデルのファイルパス
       */
      void LoadModel(std::string file_path);

      /**
       *  @brief  読み込み終了イベントを登録する
       *  @param  callback:読み込み終了時に実行する関数
       */
      void RegisterLoadedEvent(std::function<void(IModel*)> callback);

      /**
       *  @brief  ホイールイベントを発行する
       *  @param  force:ホイールの回転方向
       */
      void DispatchWheelEvent(const std::float_t force);

      /**
       *  @brief  カメラの座標を取得する
       *  @return カメラの座標ベクトル
       */
      DirectX::XMVECTOR GetCameraPosition();

      /**
       *  @brief  注視点の座標を取得する
       *  @return 注視点の座標ベクトル
       */
      DirectX::XMVECTOR GetLookAt();

    private:
      /**
       *  @brief  非同期実行処理関数
       */
      void AsyncExecute();

    private:
      std::array<actor::Actor, 10> actors_;           ///< アクターリスト
      job_system::Job job_;                           ///< 更新処理用ジョブ
      job_system::AsyncJob async_job_;                ///< 非同期処理用ジョブ
      bool initialized_ = false;                      ///< 初期化終了フラグ
      std::string file_path_;                         ///< モデルファイルパス
      std::function<void(IModel*)> function_;         ///< 関数へのポインタ
    };
  };
};