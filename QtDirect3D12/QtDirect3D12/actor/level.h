/**
 *  @file     level.h
 *  @brief    レベルに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/23
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include "job_system/job.h"
#include "job_system/async_job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  レベルクラス
     */
    class Level final
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Level();

      /**
       *  @brief  デストラクタ
       */
      ~Level();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Level(const Level& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      Level& operator = (const Level& other) = delete;

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

    private:
      /**
       *  @brief  非同期実行処理関数
       */
      void AsyncExecute();

    private:
      job_system::Job job_;             ///< 更新処理用ジョブ
      job_system::AsyncJob async_job_;  ///< 非同期処理用ジョブ
      bool initialized_ = false;        ///< 初期化終了フラグ
    };
  };
};